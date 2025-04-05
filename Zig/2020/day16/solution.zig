const std = @import("std");
const mem = std.mem;
const print = std.debug.print;
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;

const Range = struct {
    lower: i32,
    upper: i32,

    fn contains(self: *const Range, value: i32) bool {
        return self.lower <= value and value <= self.upper;
    }
};

const Field = struct {
    ranges: [2]Range = undefined,

    fn is_valid(self: *const Field, value: i32) bool {
        for (self.ranges) |range| {
            if (range.contains(value)) return true;
        }
        return false;
    }
};

const Notes = struct {
    fields: std.StringHashMap(Field) = undefined,
    ticket: []i32 = undefined,
    nearby: std.ArrayList([]i32) = undefined,
};

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const args = try std.process.argsAlloc(allocator);
    std.debug.assert(args.len > 1);
    var document = try parse_document(&allocator, args[1]);

    print("Part 1: {!}\n", .{part1(&document)});
    print("Part 2: {}\n", .{part2(document)});
}

fn part1(doc: *Notes) !i32 {
    var sum: i32 = 0;
    var i: usize = 0;
    while (i < doc.nearby.items.len) {
        const ticket = doc.nearby.items[i];
        var valid = true;
        ticket: for (ticket) |value| {
            var fields = doc.fields.valueIterator();
            while (fields.next()) |field| {
                if (field.is_valid(value)) continue :ticket;
            }
            sum += value;
            valid = false;
        }
        if (!valid) {
            _ = doc.nearby.swapRemove(i);
            continue;
        }
        i += 1;
    }
    return sum;
}

fn part2(doc: Notes) u32 {
    print("{}\n", .{doc.nearby.items.len});
    return 0;
}

fn parse_document(allocator: *const Allocator, filename: []const u8) !Notes {
    const file = try std.fs.cwd().openFile(filename, .{});
    defer file.close();

    const content = try file.reader().readAllAlloc(allocator.*, std.math.maxInt(usize));

    var doc: Notes = .{};
    doc.fields = std.StringHashMap(Field).init(allocator.*);
    doc.nearby = std.ArrayList([]i32).init(allocator.*);

    var sections = mem.tokenizeSequence(u8, content, "\n\n");
    for (0..3) |i| {
        var iter = mem.tokenizeScalar(u8, sections.next().?, '\n');
        if (i == 0) {
            while (iter.next()) |field_str| {
                var parts = mem.tokenizeSequence(u8, field_str, ": ");
                const field_name = parts.next().?;
                var ranges = mem.tokenizeAny(u8, parts.next().?, " -or");
                const r1: Range = .{ .lower = try std.fmt.parseInt(i32, ranges.next().?, 10), .upper = try std.fmt.parseInt(i32, ranges.next().?, 10) };
                const r2: Range = .{ .lower = try std.fmt.parseInt(i32, ranges.next().?, 10), .upper = try std.fmt.parseInt(i32, ranges.next().?, 10) };
                const field: Field = .{ .ranges = .{ r1, r2 } };
                try doc.fields.put(field_name, field);
            }
        } else {
            _ = iter.next(); // discard header message
            while (iter.next()) |ticket_str| {
                var ticket = std.ArrayList(i32).init(allocator.*);
                var values = mem.tokenizeScalar(u8, ticket_str, ',');
                while (values.next()) |value| {
                    try ticket.append(try std.fmt.parseInt(i32, value, 10));
                }
                if (i == 1) {
                    doc.ticket = try ticket.toOwnedSlice();
                } else {
                    try doc.nearby.append(try ticket.toOwnedSlice());
                }
            }
        }
    }

    return doc;
}
