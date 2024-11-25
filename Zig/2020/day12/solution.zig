const std = @import("std");
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;
const splitSeq = std.mem.splitSequence;

const Vec2d = struct {
    x: i32,
    y: i32,
};

const Ship = struct {
    pos: Vec2d = .{ .x = 0, .y = 0 },
    dir: Vec2d = .{ .x = 1, .y = 0 },
};

const NavigationInstruction = struct {
    const Action = enum { forward, north, south, east, west, left, right };
    const Self = @This();

    action: Action,
    value: u16,

    pub fn from_str(str: []const u8) !Self {
        var action: Action = .forward;
        var value = try std.fmt.parseInt(u16, str[1..], 10);
        switch (str[0]) {
            'F' => {},
            'N' => action = .north,
            'S' => action = .south,
            'E' => action = .east,
            'W' => action = .west,
            'L' => {
                action = .left;
                value = value / 90;
            },
            'R' => {
                action = .right;
                value = value / 90;
            },
            else => @panic("You can't park there mate!"),
        }

        return Self{ .action = action, .value = value };
    }
};

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const args = try std.process.argsAlloc(allocator);
    std.debug.assert(args.len > 1);
    const nav_str = try read_instructions(&allocator, args[1]);
    for (nav_str.items) |nav| {
        std.debug.print("{} {}\n", .{ nav.action, nav.value });
    }

    std.debug.print("Part 1: {}\n", .{part1(nav_str.items)});
    std.debug.print("Part 2: {}\n", .{part2(nav_str.items)});
}

fn part1(nav_instr: []NavigationInstruction) u32 {
    var ship: Ship = Ship{};
    for (nav_instr) |instr| {
        switch (instr.action) {
            .forward => {
                ship.pos += ship.dir;
            },
            .north => {},
            .south => {},
            .east => {},
            .west => {},
            .left => {},
            .right => {},
        }
    }
    return 0;
}

fn part2(_: []NavigationInstruction) u32 {
    return 0;
}

fn read_instructions(allocator: *const Allocator, filename: []const u8) !ArrayList(NavigationInstruction) {
    const file = try std.fs.cwd().openFile(filename, .{});
    defer file.close();

    var nav_instr = ArrayList(NavigationInstruction).init(allocator.*);

    const file_size = (try file.stat()).size;
    const content = try file.reader().readAllAlloc(allocator.*, file_size);

    var iter = splitSeq(u8, content, "\n");
    while (iter.next()) |line| {
        if (line.len == 0) break;
        try nav_instr.append(try NavigationInstruction.from_str(line));
    }

    return nav_instr;
}
