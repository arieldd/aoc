const std = @import("std");
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;
const splitSeq = std.mem.splitSequence;

const DIRS = [_]Vec2d{
    .{ .x = 1, .y = 0 },
    .{ .x = 0, .y = 1 },
    .{ .x = -1, .y = 0 },
    .{ .x = 0, .y = -1 },
};

const Vec2d = struct {
    x: i32,
    y: i32,

    fn mult(self: *const Vec2d, scalar: i32) Vec2d {
        return .{
            .x = self.x * scalar,
            .y = self.y * scalar,
        };
    }

    fn add(self: *const Vec2d, other: *const Vec2d) Vec2d {
        return .{
            .x = self.x + other.x,
            .y = self.y + other.y,
        };
    }
};

const Ship = struct {
    pos: Vec2d = .{ .x = 0, .y = 0 },
    dir: u16 = 0,
    waypoint: Vec2d = .{ .x = 10, .y = -1 },
};

const NavigationInstruction = struct {
    const Action = enum { forward, north, south, east, west, left, right };
    const Self = @This();

    action: Action,
    value: i16,

    pub fn from_str(str: []const u8) !Self {
        var action: Action = .forward;
        var value = try std.fmt.parseInt(i16, str[1..], 10);
        switch (str[0]) {
            'F' => {},
            'N' => action = .north,
            'S' => action = .south,
            'E' => action = .east,
            'W' => action = .west,
            'L' => {
                action = .left;
                value = @divExact(value, 90);
            },
            'R' => {
                action = .right;
                value = @divExact(value, 90);
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

    std.debug.print("Part 1: {}\n", .{part1(nav_str.items)});
    std.debug.print("Part 2: {}\n", .{part2(nav_str.items)});
}

fn part1(nav_instr: []NavigationInstruction) u32 {
    var ship: Ship = Ship{};
    for (nav_instr) |instr| {
        switch (instr.action) {
            .forward => {
                ship.pos = ship.pos.add(&DIRS[ship.dir].mult(instr.value));
            },
            .north => {
                ship.pos = ship.pos.add(&.{ .x = 0, .y = -instr.value });
            },
            .south => {
                ship.pos = ship.pos.add(&.{ .x = 0, .y = instr.value });
            },
            .east => {
                ship.pos = ship.pos.add(&.{ .x = instr.value, .y = 0 });
            },
            .west => {
                ship.pos = ship.pos.add(&.{ .x = -instr.value, .y = 0 });
            },
            .left => {
                ship.dir = @mod(ship.dir + (4 - @as(u16, @intCast(instr.value))), 4);
            },
            .right => {
                ship.dir = @mod(ship.dir + @as(u16, @intCast(instr.value)), 4);
            },
        }
    }
    return @abs(ship.pos.x) + @abs(ship.pos.y);
}

fn part2(nav_instr: []NavigationInstruction) u32 {
    var ship: Ship = Ship{};
    for (nav_instr) |instr| {
        switch (instr.action) {
            .forward => {
                ship.pos = ship.pos.add(&ship.waypoint.mult(instr.value));
            },
            .north => {
                ship.waypoint = ship.waypoint.add(&.{ .x = 0, .y = -instr.value });
            },
            .south => {
                ship.waypoint = ship.waypoint.add(&.{ .x = 0, .y = instr.value });
            },
            .east => {
                ship.waypoint = ship.waypoint.add(&.{ .x = instr.value, .y = 0 });
            },
            .west => {
                ship.waypoint = ship.waypoint.add(&.{ .x = -instr.value, .y = 0 });
            },
            else => {
                const rota = sin_cos((if (instr.action == .right) instr.value else -instr.value) * 90);
                const nx: i32 = ship.waypoint.x * rota.cos - ship.waypoint.y * rota.sin;
                const ny: i32 = ship.waypoint.x * rota.sin + ship.waypoint.y * rota.cos;
                ship.waypoint = .{ .x = nx, .y = ny };
            },
        }
    }
    return @abs(ship.pos.x) + @abs(ship.pos.y);
}

fn sin_cos(degrees: i32) struct { sin: i32, cos: i32 } {
    const theta = @as(f32, @floatFromInt(degrees)) * (std.math.pi / 180.0);
    return .{
        .sin = @as(i32, @intFromFloat(@sin(theta))),
        .cos = @as(i32, @intFromFloat(@cos(theta))),
    };
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
