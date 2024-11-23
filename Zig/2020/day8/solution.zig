const std = @import("std");
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;
const splitSeq = std.mem.splitSequence;

const Instr = struct {
    const InstrType = enum(u8) { noop, acc, jmp };

    op: InstrType = .noop,
    arg: i32,

    pub fn print(self: *const Instr) void {
        std.debug.print("{} {}\n", .{ self.op, self.arg });
    }
};

fn read_program(allocator: *const Allocator, lines: [][]const u8) !ArrayList(Instr) {
    var program: ArrayList(Instr) = ArrayList(Instr).init(allocator.*);
    for (lines) |l| {
        var iter = std.mem.splitScalar(u8, l, ' ');
        const op_str = iter.first();
        const arg = try std.fmt.parseInt(i32, iter.next().?, 10);
        var next_instr: Instr = .{ .arg = arg };

        if (std.mem.eql(u8, op_str, "acc")) {
            next_instr.op = .acc;
        } else if (std.mem.eql(u8, op_str, "jmp")) {
            next_instr.op = .jmp;
        }
        try program.append(next_instr);
    }
    return program;
}

fn solve(allocator: *const Allocator, program: *const ArrayList(Instr)) !struct { i32, i32 } {
    var instr_counter: i32 = 0;
    var accumulator: i32 = 0;

    var executed_instr: std.AutoHashMap(i32, void) = std.AutoHashMap(i32, void).init(allocator.*);
    while (instr_counter < program.items.len) {
        if (executed_instr.contains(instr_counter))
            break;

        try executed_instr.put(instr_counter, {});
        const instr = program.items[@intCast(instr_counter)];
        switch (instr.op) {
            .noop => {},
            .acc => accumulator += instr.arg,
            .jmp => {
                instr_counter += instr.arg;
                continue;
            },
        }
        instr_counter += 1;
    }
    return .{ accumulator, instr_counter };
}

fn part2(allocator: *const Allocator, program: *const ArrayList(Instr)) !i32 {
    var program_modified = try program.clone();
    for (program_modified.items, 0..) |instr, i| {
        var new_instr: Instr = .{ .arg = instr.arg };
        switch (instr.op) {
            .acc => continue,
            .noop => {
                new_instr.op = .jmp;
            },
            .jmp => {
                // New instr is .noop by default
            },
        }

        program_modified.items[i] = new_instr;
        const run = try solve(allocator, &program_modified);
        if (run[1] == program.items.len) {
            return run[0];
        }
        program_modified.items[i] = instr;
    }
    return 0;
}

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const args = try std.process.argsAlloc(allocator);
    std.debug.assert(args.len > 1);

    const lines = try read_lines(&allocator, args[1]);
    const program = try read_program(&allocator, lines.items);

    const p1 = try solve(&allocator, &program);
    std.debug.print("Part 1: {}\n", .{p1[0]});
    std.debug.print("Part 2: {!}\n", .{part2(&allocator, &program)});
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
