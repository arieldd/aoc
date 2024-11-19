const std = @import("std");
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;

const splitSeq = std.mem.splitSequence;
const eql = std.mem.eql;

const Colors = [_][]const u8{
    "amb",
    "blu",
    "brn",
    "gry",
    "grn",
    "hzl",
    "oth",
};

const Field = struct {
    const total = 8;
    const names = enum { byr, iyr, eyr, hgt, hcl, ecl, pid, cid };

    key: []const u8,
    value: []const u8,

    pub fn isValid(self: *const Field) bool {
        if (eql(u8, self.key, @tagName(.cid)))
            return true;

        if (eql(u8, self.key, @tagName(.byr))) {
            const number = std.fmt.parseInt(u86, self.value, 10) catch return false;
            return 1920 <= number and number <= 2002;
        }
        if (eql(u8, self.key, @tagName(.iyr))) {
            const number = std.fmt.parseInt(u16, self.value, 10) catch return false;

            return 2010 <= number and number <= 2020;
        }
        if (eql(u8, self.key, @tagName(.eyr))) {
            const number = std.fmt.parseInt(u86, self.value, 10) catch return false;
            return 2020 <= number and number <= 2030;
        }
        if (eql(u8, self.key, @tagName(.hgt))) {
            const len = self.value.len;
            if (len < 3) return false;
            const measure = self.value[len - 2 ..];
            const number = std.fmt.parseInt(u8, self.value[0 .. len - 2], 10) catch return false;
            if (eql(u8, measure, "cm")) {
                return 150 <= number and number <= 193;
            }

            if (eql(u8, measure, "in")) {
                return 59 <= number and number <= 76;
            }
            return false;
        }
        if (eql(u8, self.key, @tagName(.hcl))) {
            if (self.value.len < 2 or self.value[0] != '#')
                return false;
            const hex = self.value[1..];
            for (hex) |c| {
                if (c < '0' or (c > '9' and c < 'a') or c > 'f')
                    return false;
            }

            return true;
        }
        if (eql(u8, self.key, @tagName(.ecl))) {
            for (Colors) |color| {
                if (eql(u8, color, self.value))
                    return true;
            }
            return false;
        }
        if (eql(u8, self.key, @tagName(.pid))) {
            if (self.value.len != 9) return false;
            for (self.value) |d| {
                if (!std.ascii.isDigit(d))
                    return false;
            }
            return true;
        }

        return false;
    }
};

const Passport = struct {
    fields: [Field.total]?Field = .{null} ** Field.total,
    fields_present: u8 = 0,

    pub fn addField(self: *Passport, field: Field) void {
        std.debug.assert(self.fields_present < Field.total);

        self.fields[self.fields_present] = field;
        self.fields_present += 1;
    }

    pub fn isValid(self: *const Passport, strict: bool) bool {
        if (strict) {
            for (0..self.fields_present) |i| {
                if (!self.fields[i].?.isValid()) {
                    return false;
                }
            }
        }

        if (self.fields_present == Field.total) return true;

        if (self.fields_present < Field.total - 1) return false;

        for (0..self.fields_present) |i| {
            if (std.mem.eql(u8, self.fields[i].?.key, @tagName(.cid))) {
                return false;
            }
        }

        return true;
    }
};

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const lines = try read_lines(&allocator, "input.txt");

    const passports = try parse_input(&allocator, lines.items);
    std.debug.print("Part 1: {}\n", .{solve(passports, false)});
    std.debug.print("Part 2: {}\n", .{solve(passports, true)});
}

fn solve(passports: []const Passport, strict: bool) u32 {
    var result: u32 = 0;
    for (passports) |p| {
        if (p.isValid(strict)) {
            result += 1;
        }
    }
    return result;
}

fn parse_input(allocator: *const Allocator, lines: [][]const u8) ![]Passport {
    var passports: []Passport = try allocator.alloc(Passport, lines.len);
    for (lines, 0..) |l, i| {
        passports[i] = .{};

        var iter = std.mem.splitAny(u8, l, " \n");
        while (iter.next()) |data| {
            if (data.len == 0) break;
            var pair = std.mem.splitScalar(u8, data, ':');
            passports[i].addField(.{ .key = pair.next().?, .value = pair.next().? });
        }
    }
    return passports;
}

fn read_lines(allocator: *const Allocator, filename: []const u8) !ArrayList([]const u8) {
    const file = try std.fs.cwd().openFile(filename, .{});
    defer file.close();

    var lines = ArrayList([]const u8).init(allocator.*);

    const file_size = (try file.stat()).size;
    const content = try file.reader().readAllAlloc(allocator.*, file_size);

    var iter = splitSeq(u8, content, "\n\n");
    while (iter.next()) |line| {
        try lines.append(line);
    }

    return lines;
}
