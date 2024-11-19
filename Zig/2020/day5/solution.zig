const std = @import("std");
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;
const splitSeq = std.mem.splitSequence;

const BoardingPass = struct {
    row: [7]u1 = .{0} ** 7,
    seat: [3]u1 = .{0} ** 3,

    pub fn ID(self: *const BoardingPass) u64 {
        return toDecimal(&self.row) * 8 + toDecimal(&self.seat);
    }
};

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const lines = try readLines(&allocator, "input.txt");
    const passes = try readBoardingPasses(&allocator, lines.items);

    std.debug.print("Part 1: {}\n", .{part1(&passes)});
    std.debug.print("Part 2: {!}\n", .{part2(&allocator, &passes)});
}

fn part1(passes: *const ArrayList(BoardingPass)) u64 {
    var maxId: u64 = 0;
    for (passes.items) |p| {
        const id = p.ID();
        if (id > maxId) {
            maxId = id;
        }
    }
    return maxId;
}

fn part2(allocator: *const Allocator, passes: *const ArrayList(BoardingPass)) !u64 {
    var pass_ids: std.AutoHashMap(u64, void) = std.AutoHashMap(u64, void).init(allocator.*);
    for (passes.items) |p| {
        try pass_ids.put(p.ID(), void{});
    }

    //Not front not back
    var row: u32 = 1;
    while (row < 127) : (row += 1) {
        var column: u32 = 0;
        while (column < 8) : (column += 1) {
            const id = row * 8 + column;
            if (!pass_ids.contains(id) and pass_ids.contains(id - 1) and pass_ids.contains(id + 1)) {
                return id;
            }
        }
    }

    return 0;
}

fn toDecimal(bits: []const u1) u64 {
    var decimal: u64 = 0;

    var i: usize = 0;
    while (i < bits.len) : (i += 1) {
        if (bits[bits.len - 1 - i] == 1)
            decimal += std.math.pow(u64, 2, i);
    }
    return decimal;
}

fn readBoardingPasses(allocator: *const Allocator, lines: [][]const u8) !ArrayList(BoardingPass) {
    var passes: ArrayList(BoardingPass) = ArrayList(BoardingPass).init(allocator.*);
    for (lines) |value| {
        var pass: BoardingPass = .{};
        for (value[0..7], 0..) |row, i| {
            if (row == 'B')
                pass.row[i] = 1;
        }
        for (value[7..], 0..) |column, i| {
            if (column == 'R')
                pass.seat[i] = 1;
        }
        try passes.append(pass);
    }
    return passes;
}

fn readLines(allocator: *const Allocator, filename: []const u8) !ArrayList([]const u8) {
    const file = try std.fs.cwd().openFile(filename, .{});
    defer file.close();

    var lines = ArrayList([]const u8).init(allocator.*);

    const file_size = (try file.stat()).size;
    const content = try file.reader().readAllAlloc(allocator.*, file_size);

    var iter = splitSeq(u8, content, "\n");
    while (iter.next()) |line| {
        if (line.len == 0) break;
        try lines.append(line);
    }

    return lines;
}
