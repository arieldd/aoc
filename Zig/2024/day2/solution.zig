const std = @import("std");
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;
const mem = std.mem;
const print = std.debug.print;

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const args = try std.process.argsAlloc(allocator);
    std.debug.assert(args.len > 1);
    const reports = try read_reports(&allocator, args[1]);

    print("Part 1: {}\n", .{part1(reports)});
    print("Part 2: {!}\n", .{part2(reports)});
}

fn check_safe(report: []i32) bool {
    var safe: bool = true;
    var monotony: i8 = 0;
    var current: i32 = report[0];
    var i: usize = 1;
    while (i < report.len) : (i += 1) {
        const next: i32 = report[i];
        const diff: i32 = next - current;
        const sign: i8 = if (diff > 0) 1 else -1;
        if ((@abs(diff) > 3 or @abs(diff) < 1) or (monotony != 0 and sign != monotony)) {
            safe = false;
            break;
        }

        monotony = sign;
        current = next;
    }
    return safe;
}

fn part1(reports: ArrayList([]i32)) i32 {
    var count: i32 = 0;
    for (reports.items) |report| {
        if (check_safe(report))
            count += 1;
    }
    return count;
}

fn part2(reports: ArrayList([]i32)) !i32 {
    var count: i32 = 0;
    const allocator = reports.allocator;
    for (reports.items) |report| {
        if (check_safe(report)) {
            count += 1;
        } else {
            const n: usize = report.len;
            var i: usize = 0;
            while (i < n) : (i += 1) {
                const temp: []i32 = try allocator.alloc(i32, n - 1);
                defer allocator.free(temp);

                var index: usize = 0;
                for (report, 0..) |value, j| {
                    if (i == j) continue;
                    temp[index] = value;
                    index += 1;
                }
                if (check_safe(temp)) {
                    count += 1;
                    break;
                }
            }
        }
    }
    return count;
}

fn read_reports(allocator: *const Allocator, filename: []const u8) !ArrayList([]i32) {
    const file = try std.fs.cwd().openFile(filename, .{});
    defer file.close();

    var reports = ArrayList([]i32).init(allocator.*);

    const file_size = (try file.stat()).size;
    const content = try file.reader().readAllAlloc(allocator.*, file_size);

    var iter = mem.splitScalar(u8, content, '\n');
    while (iter.next()) |line| {
        if (line.len == 0) break;
        var values = ArrayList(i32).init(allocator.*);
        var v_iter = mem.splitScalar(u8, line, ' ');
        while (v_iter.next()) |value| {
            const number = std.fmt.parseInt(i32, value, 10) catch continue;
            try values.append(number);
        }
        try reports.append(try values.toOwnedSlice());
    }

    return reports;
}
