const std = @import("std");
const mem = std.mem;
const print = std.debug.print;
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;

const Move = struct {
    dir: i4 = undefined,
    size: i32 = undefined,
};

const Point = struct { x: i32 = 0, y: i32 = 0 };

const Taxi = struct {
    pos: Point = .{},
    facing: i4 = 0,
};

const adj4 = [4]struct { i4, i4 }{ .{ 0, -1 }, .{ 1, 0 }, .{ 0, 1 }, .{ -1, 0 } };

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const args = try std.process.argsAlloc(allocator);
    std.debug.assert(args.len > 1);
    const moves = try read_lines(&allocator, args[1]);

    print("Part 1: {!}\n", .{solve(&allocator, moves.items, .{})});
}

fn solve(allocator: *const Allocator, moves: []const Move, start: Taxi) !struct { u32, u32 } {
    var current: Taxi = start;
    var visited: std.AutoHashMap(Point, void) = std.AutoHashMap(Point, void).init(allocator.*);
    var found: bool = false;

    var part2: u32 = 0;
    try visited.put(.{ .x = start.pos.x, .y = start.pos.y }, {});
    for (moves) |move| {
        current.facing = @mod(current.facing + move.dir, 4);
        const idx = @as(usize, @intCast(current.facing));

        var step: i32 = 0;
        while (step < move.size) : (step += 1) {
            current.pos.x += adj4[idx][0];
            current.pos.y += adj4[idx][1];

            if (visited.contains(current.pos)) {
                if (!found) {
                    part2 = @abs(current.pos.x) + @abs(current.pos.y);
                    found = true;
                }
            } else {
                try visited.put(current.pos, {});
            }
        }
    }
    return .{ @abs(current.pos.x) + @abs(current.pos.y), part2 };
}

fn read_lines(allocator: *const Allocator, filename: []const u8) !ArrayList(Move) {
    const file = try std.fs.cwd().openFile(filename, .{});
    defer file.close();

    var moves = ArrayList(Move).init(allocator.*);

    const file_size = (try file.stat()).size;
    const content = try file.reader().readAllAlloc(allocator.*, file_size);

    var lines = mem.splitScalar(u8, content, '\n');
    var iter = mem.splitSequence(u8, lines.first(), ", ");
    while (iter.next()) |move| {
        if (move.len == 0) break;

        var next: Move = .{};
        switch (move[0]) {
            'R' => next.dir = 1,
            'L' => next.dir = 3,
            else => @panic("Woops"),
        }
        next.size = try std.fmt.parseInt(i32, move[1..], 10);
        try moves.append(next);
    }

    return moves;
}
