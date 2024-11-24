const std = @import("std");
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;
const splitSeq = std.mem.splitSequence;

const dx = [_]i8{ 1, 1, 0, -1, -1, -1, 0, 1 };
const dy = [_]i8{ 0, 1, 1, 1, 0, -1, -1, -1 };

const Round = struct {
    flipped: u32 = 0,
    seats: ArrayList([]const u8) = undefined,

    pub fn init(allocator: *const Allocator) Round {
        return .{
            .seats = ArrayList([]const u8).init(allocator.*),
        };
    }
};

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const args = try std.process.argsAlloc(allocator);
    std.debug.assert(args.len > 1);
    const seats = try read_seats(&allocator, args[1]);

    std.debug.print("Part 1: {}\n", .{solve(&allocator, seats, false)});
    std.debug.print("Part 2: {}\n", .{solve(&allocator, seats, true)});
}

fn solve(allocator: *const Allocator, grid: ArrayList([]const u8), new_rules: bool) u32 {
    var rounds: u32 = 0;
    var round: Round = .{ .seats = grid };
    while (true) : (rounds += 1) {
        round = simulate_round(allocator, round.seats.items, new_rules);
        if (round.flipped == 0) break;
    }

    var occuppied: u32 = 0;
    for (round.seats.items) |row| {
        for (row) |seat| {
            if (seat == '#')
                occuppied += 1;
        }
    }
    return occuppied;
}

fn simulate_round(allocator: *const Allocator, grid: [][]const u8, new_rules: bool) Round {
    const n = grid.len;
    const m = grid[0].len;

    const tolerance: u8 = if (new_rules) 5 else 4;

    var i: u32 = 0;
    var round: Round = Round.init(allocator);
    while (i < n) : (i += 1) {
        var j: u32 = 0;
        var row: []u8 = allocator.alloc(u8, m) catch return .{};
        while (j < m) : (j += 1) {
            var value: u8 = grid[i][j];
            const count: u8 = count_occupied(grid, i, j, n, m, new_rules);
            if (grid[i][j] == '#' and count >= tolerance) {
                value = 'L';
                round.flipped += 1;
            } else if (grid[i][j] == 'L') {
                if (count == 0) {
                    value = '#';
                    round.flipped += 1;
                }
            }
            row[j] = value;
        }
        round.seats.append(row) catch return .{};
    }

    return round;
}

fn count_occupied(grid: [][]const u8, i: u32, j: u32, n: usize, m: usize, new_rules: bool) u8 {
    var occuppied: u8 = 0;
    comptime var k = 0;
    inline while (k < 8) : (k += 1) {
        var ni: i32 = @as(i32, @intCast(i));
        var nj: i32 = @as(i32, @intCast(j));
        while (true) {
            ni += dy[k];
            nj += dx[k];
            if (!is_valid(ni, nj, n, m)) break;
            if (grid[@as(u32, @intCast(ni))][@as(u32, @intCast(nj))] == '#') {
                occuppied += 1;
                break;
            }

            if (grid[@as(u32, @intCast(ni))][@as(u32, @intCast(nj))] == 'L') {
                break;
            }

            if (!new_rules) break;
        }
    }
    return occuppied;
}

fn is_valid(i: i32, j: i32, n: usize, m: usize) bool {
    return i >= 0 and i < n and j >= 0 and j < m;
}

fn read_seats(allocator: *const Allocator, filename: []const u8) !ArrayList([]const u8) {
    const file = try std.fs.cwd().openFile(filename, .{});
    defer file.close();

    var seats = ArrayList([]const u8).init(allocator.*);

    const file_size = (try file.stat()).size;
    const content = try file.reader().readAllAlloc(allocator.*, file_size);

    var iter = std.mem.splitScalar(u8, content, '\n');
    while (iter.next()) |line| {
        if (line.len == 0) break;
        try seats.append(line);
    }

    return seats;
}
