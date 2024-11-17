const std = @import("std");
const Allocator = std.mem.Allocator;
const split = std.mem.splitSequence;

const TOTAL_FIELDS = 8;

const Field = struct {
    key: []const u8,
    value: []const u8,

    pub fn isValid(_: *const Field) bool {
        return false;
    }
};

const Passport = struct {
    fields: [TOTAL_FIELDS]?Field = .{null} ** TOTAL_FIELDS,
    fields_present: u8 = 0,

    pub fn addField(self: *Passport, field: Field) void {
        std.debug.assert(self.fields_present < TOTAL_FIELDS);

        self.fields[self.fields_present] = field;
        self.fields_present += 1;
    }

    pub fn isValid(self: *const Passport) bool {
        if (self.fields_present == TOTAL_FIELDS) return true;

        if (self.fields_present < TOTAL_FIELDS - 1) return false;

        for (0..self.fields_present) |i| {
            if (std.mem.eql(u8, self.fields[i].?.key, "cid")) {
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

    const passports = try parse_input(&allocator, lines);
    std.debug.print("Part 1: {}\n", .{part1(passports)});
    std.debug.print("Part 2: {}\n", .{part2(lines)});
}

fn part1(passports: []const Passport) u32 {
    var result: u32 = 0;
    for (passports) |p| {
        if (p.isValid()) {
            result += 1;
        }
    }
    return result;
}

fn part2(_: [][]const u8) u32 {
    return 0;
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

fn read_lines(allocator: *const Allocator, filename: []const u8) ![][]const u8 {
    const file = try std.fs.cwd().openFile(filename, .{ .mode = .read_only });
    defer file.close();

    const file_size = (try file.stat()).size;
    const input = try file.readToEndAlloc(allocator.*, file_size);

    const initial_lines = 100;
    var lines: [][]const u8 = try allocator.alloc([]u8, initial_lines);

    var iter = split(u8, input, "\n\n");

    var i: usize = 0;
    while (iter.next()) |line| : (i += 1) {
        if (line.len == 0) break;

        if (i == lines.len) {
            lines = try allocator.realloc(lines, lines.len + initial_lines);
        }
        lines[i] = line;
    }

    if (i < lines.len) {
        lines = try allocator.realloc(lines, i);
    }

    return lines;
}
