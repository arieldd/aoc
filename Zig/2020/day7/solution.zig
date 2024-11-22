const std = @import("std");
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;
const splitSeq = std.mem.splitSequence;

const MyBag = "shiny gold";

const Bag = struct {
    colour: []const u8 = undefined,
    contents: std.StringHashMap(u16) = undefined,

    pub fn print(self: *Bag) void {
        std.debug.print("{s}-> ", .{self.colour});
        var contents = self.contents.iterator();
        while (contents.next()) |entry| {
            std.debug.print("{s}:{d} ", .{ entry.key_ptr.*, entry.value_ptr.* });
        }
        std.debug.print("\n", .{});
    }
};

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const args = try std.process.argsAlloc(allocator);
    std.debug.assert(args.len > 1);
    const lines = try read_lines(&allocator, args[1]);
    const bags = try parse_input(&allocator, lines.items);

    std.debug.print("Part 1: {}\n", .{part1(&allocator, &bags)});
    std.debug.print("Part 2: {}\n", .{part2(&bags)});
}

fn part1(allocator: *const Allocator, bags: *const std.StringHashMap(Bag)) u32 {
    var found_bags: std.StringHashMap(void) = std.StringHashMap(void).init(allocator.*);
    var visited_bags: std.StringHashMap(void) = std.StringHashMap(void).init(allocator.*);
    var bags_iter = bags.valueIterator();
    while (bags_iter.next()) |bag| {
        if (!visited_bags.contains(bag.colour)) {
            find_containing_bags(bags, bag.colour, MyBag, &found_bags, &visited_bags) catch {
                @panic("oopsie");
            };
        }
    }
    return found_bags.count();
}

fn part2(bags: *const std.StringHashMap(Bag)) u32 {
    return count_bags_inside(bags, MyBag);
}

fn count_bags_inside(bags: *const std.StringHashMap(Bag), current_colour: []const u8) u32 {
    const bag: Bag = bags.get(current_colour).?;

    var total: u32 = 0;
    var contents = bag.contents.iterator();
    while (contents.next()) |entry| {
        const next_colour = entry.key_ptr.*;
        const value = entry.value_ptr.*;

        total += value + value * count_bags_inside(bags, next_colour);
    }
    return total;
}

fn find_containing_bags(bags: *const std.StringHashMap(Bag), current_colour: []const u8, target: []const u8, found_bags: *std.StringHashMap(void), visited_bags: *std.StringHashMap(void)) !void {
    if (std.mem.eql(u8, target, current_colour)) {
        return;
    }

    try visited_bags.put(current_colour, {});

    var bag_contents = bags.get(current_colour).?.contents.iterator();
    while (bag_contents.next()) |bag| {
        const next_colour = bag.key_ptr.*;
        if (std.mem.eql(u8, target, next_colour)) {
            if (!found_bags.contains(current_colour)) {
                try found_bags.put(current_colour, {});
            }
        } else if (!visited_bags.contains(next_colour)) {
            try find_containing_bags(bags, next_colour, target, found_bags, visited_bags);
        }

        if (found_bags.contains(next_colour)) {
            try found_bags.put(current_colour, {});
        }
    }
}

fn parse_input(allocator: *const Allocator, lines: [][]const u8) !std.StringHashMap(Bag) {
    var bags: std.StringHashMap(Bag) = std.StringHashMap(Bag).init(allocator.*);
    for (lines) |bag_str| {
        var comma_iter = splitSeq(u8, bag_str, ", ");

        var next_bag: Bag = .{};
        var i: u32 = 0;

        const first_chunk = comma_iter.first();
        var space_iter = std.mem.splitScalar(u8, first_chunk, ' ');
        next_bag.colour = try std.fmt.allocPrint(allocator.*, "{s} {s}", .{ space_iter.first(), space_iter.next().? });
        next_bag.contents = std.StringHashMap(u16).init(allocator.*);

        while (space_iter.next()) |word| {
            const value = std.fmt.parseInt(u8, word, 10) catch continue;
            const bag_color = try std.fmt.allocPrint(allocator.*, "{s} {s}", .{ space_iter.next().?, space_iter.next().? });
            try next_bag.contents.put(bag_color, value);
            break; // No need to read 'bags'
        }

        while (comma_iter.next()) |chunk| : (i += 1) {
            space_iter = std.mem.splitScalar(u8, chunk, ' ');

            const value = std.fmt.parseInt(u8, space_iter.first(), 10) catch @panic("You can't park there mate!");
            const bag_color = try std.fmt.allocPrint(allocator.*, "{s} {s}", .{ space_iter.next().?, space_iter.next().? });
            try next_bag.contents.put(bag_color, value);
            continue; // No need to read 'bags'
        }
        _ = try bags.put(next_bag.colour, next_bag);
    }
    return bags;
}

fn read_lines(allocator: *const Allocator, filename: []const u8) !ArrayList([]const u8) {
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
