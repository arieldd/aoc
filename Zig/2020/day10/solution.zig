const std = @import("std");
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;
const splitSeq = std.mem.splitSequence;

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const args = try std.process.argsAlloc(allocator);
    std.debug.assert(args.len > 1);
    const numbers = try read_lines(&allocator, args[1]);

    std.debug.print("Part 1: {}\n", .{part1(&numbers)});
    std.debug.print("Part 2: {}\n", .{part2(&numbers)});
}

fn part1(numbers: *const ArrayList(i64)) u64 {
    const devices = numbers.clone() catch return 0;
    std.mem.sort(i64, devices.items, {}, comptime std.sort.asc(i64));
    var ones: u64 = 0;
    var threes: u64 = 1;

    if (devices.items[0] == 1) ones += 1;
    if (devices.items[0] == 3) ones += 3;

    var i: usize = 1;
    while (i < devices.items.len) : (i += 1) {
        const jolt = devices.items[i];
        const prev_jolt = devices.items[i - 1];
        if (jolt - prev_jolt == 1) ones += 1;
        if (jolt - prev_jolt == 3) threes += 1;
    }

    return ones * threes;
}

fn part2(numbers: *const ArrayList(i64)) i64 {
    const devices = numbers.clone() catch return 0;
    std.mem.sort(i64, devices.items, {}, comptime std.sort.asc(i64));

    var dp: std.AutoHashMap(i64, i64) = std.AutoHashMap(i64, i64).init(devices.allocator);
    return count_arrangements(&devices, 0, devices.items[devices.items.len - 1] + 3, &dp);
}

fn count_arrangements(devices: *const ArrayList(i64), joltage: i64, max_joltage: i64, dp: *std.AutoHashMap(i64, i64)) i64 {
    if (dp.contains(joltage))
        return dp.get(joltage).?;

    var result: i64 = 0;
    const usable: ArrayList(i64) = get_usable_devices(&devices.allocator, devices.items, joltage);

    if (usable.items.len == 0) return if (max_joltage - joltage <= 3) 1 else 0;

    for (usable.items) |device| {
        result += count_arrangements(devices, device, max_joltage, dp);
    }

    dp.put(joltage, result) catch return 0;
    return result;
}

fn get_usable_devices(allocator: *const Allocator, devices: []const i64, joltage: i64) ArrayList(i64) {
    var result: ArrayList(i64) = ArrayList(i64).init(allocator.*);

    var i: usize = 0;
    while (i < devices.len) : (i += 1) {
        if (devices[i] <= joltage) continue;
        if (devices[i] - joltage <= 3) {
            result.append(devices[i]) catch break;
        } else break;
    }

    return result;
}

fn read_lines(allocator: *const Allocator, filename: []const u8) !ArrayList(i64) {
    const file = try std.fs.cwd().openFile(filename, .{});
    defer file.close();

    var numbers = ArrayList(i64).init(allocator.*);

    const file_size = (try file.stat()).size;
    const content = try file.reader().readAllAlloc(allocator.*, file_size);

    var iter = splitSeq(u8, content, "\n");
    while (iter.next()) |line| {
        if (line.len == 0) break;
        try numbers.append(try std.fmt.parseInt(i64, line, 10));
    }

    return numbers;
}
