const std = @import("std");
const print = std.debug.print;

const input = @embedFile("input.txt");

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();

    const bits = try get_bit_stream(input, &allocator);
    defer allocator.free(bits);

    const packet = Packet.parse(bits).?;
    print("Part 1: {}\n", .{packet.get_version_sum()});
    print("Part 2: {}\n", .{packet.eval()});
}

const Packet = struct {
    version: u3 = undefined,
    typeID: u3 = undefined,
    length: usize = 6,
    value: ?usize = null,
    packets: ?[]Packet = null,

    fn parse(bits: []const u1) ?Packet {
        std.debug.assert(bits.len >= 7);

        var result: Packet = .{
            .version = to_decimal(u3, bits[0..3]), //
            .typeID = to_decimal(u3, bits[3..6]),
        };

        switch (result.typeID) {
            4 => result.read_value(bits[6..]) catch {},
            else => result.parse_subpackets(bits[6..]) catch {},
        }

        return result;
    }

    fn get_version_sum(self: *const Packet) usize {
        var sum: usize = self.version;
        if (self.packets != null) {
            for (self.packets.?) |p| {
                sum += p.get_version_sum();
            }
        }
        return sum;
    }

    fn eval(self: *const Packet) usize {
        switch (self.typeID) {
            0 => {
                var sum: usize = 0;
                for (self.packets.?) |p| {
                    sum += p.eval();
                }
                return sum;
            },
            1 => {
                var prod: usize = 1;
                for (self.packets.?) |p| {
                    prod *= p.eval();
                }
                return prod;
            },
            2 => {
                var min: usize = std.math.maxInt(usize);
                for (self.packets.?) |p| {
                    min = @min(min, p.eval());
                }
                return min;
            },
            3 => {
                var max: usize = 0;
                for (self.packets.?) |p| {
                    max = @max(max, p.eval());
                }
                return max;
            },
            4 => return self.value.?,
            5 => return if (self.packets.?[0].eval() > self.packets.?[1].eval()) 1 else 0,
            6 => return if (self.packets.?[0].eval() < self.packets.?[1].eval()) 1 else 0,
            7 => return if (self.packets.?[0].eval() == self.packets.?[1].eval()) 1 else 0,
        }
    }

    fn read_value(self: *Packet, bits: []const u1) !void {
        std.debug.assert(bits.len >= 5);

        var buffer: [256]u8 = undefined;
        var fba = std.heap.FixedBufferAllocator.init(&buffer);
        const allocator = fba.allocator();

        var value_bits = std.ArrayList(u1).init(allocator);
        defer value_bits.clearAndFree();

        var group_index: usize = 0;
        while (group_index <= bits.len) : (group_index += 5) {
            const group = bits[group_index .. group_index + 5];
            self.length += 5;

            try value_bits.appendSlice(group[1..]);

            if (group[0] == 0)
                break;
        }

        self.value = to_decimal(usize, value_bits.items);
    }

    fn parse_subpackets(self: *Packet, bits: []const u1) !void {
        std.debug.assert(bits.len > 1);

        self.length += 1;

        var gpa = std.heap.GeneralPurposeAllocator(.{}){};
        const allocator = gpa.allocator();

        var subpackets = std.ArrayList(Packet).init(allocator);
        switch (bits[0]) {
            0 => {
                const size = to_decimal(usize, bits[1..16]);
                self.length += 15 + size;
                var read: usize = 0;
                while (read < size) {
                    const next = Packet.parse(bits[16 + read ..]).?;
                    try subpackets.append(next);
                    read += next.length;
                }
            },
            1 => {
                const count = to_decimal(usize, bits[1..12]);
                self.length += 11;
                var read: usize = 0;
                var offset: usize = 0;
                while (read < count) : (read += 1) {
                    const next = Packet.parse(bits[12 + offset ..]).?;
                    offset += next.length;
                    self.length += next.length;

                    try subpackets.append(next);
                }
            },
        }

        // TODO: This a leak as there is no way to free this memory after.
        // I should pass the allocator to the packet init call
        self.packets = try subpackets.toOwnedSlice();
    }
};

fn get_bit_stream(data: []const u8, allocator: *const std.mem.Allocator) ![]u1 {
    var bits: []u1 = try allocator.alloc(u1, data.len * 4);
    for (data, 0..) |c, i| {
        if (c < '0')
            break; //ignore newline at end of file
        const ascii = try std.fmt.charToDigit(c, 16);
        for (0..4) |j| {
            bits[i * 4 + j] = @truncate(ascii >> @truncate(3 - j));
        }
    }
    return bits;
}

fn to_decimal(comptime T: type, bits: []const u1) T {
    var result: T = 0;
    for (bits) |b| {
        result *= 2;
        result += b;
    }

    return result;
}

test "Should parse bitstream" {
    const t1 = try get_bit_stream("D2FE28", &std.testing.allocator);
    defer std.testing.allocator.free(t1);
    for (t1, "110100101111111000101000") |actual, expected| {
        try std.testing.expectEqual(actual, expected - '0');
    }

    const t2 = try get_bit_stream("38006F45291200", &std.testing.allocator);
    defer std.testing.allocator.free(t2);
    for (t2, "00111000000000000110111101000101001010010001001000000000") |actual, expected| {
        try std.testing.expectEqual(actual, expected - '0');
    }
}

test "Should convert to_decimal" {
    const t1 = to_decimal(usize, &[_]u1{ 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1 });
    try std.testing.expectEqual(2021, t1);
}

test "Should parse literal packet" {
    const bits = try get_bit_stream("D2FE28", &std.testing.allocator);
    defer std.testing.allocator.free(bits);
    const packet = Packet.parse(bits).?;

    try std.testing.expectEqual(6, packet.version);
    try std.testing.expectEqual(4, packet.typeID);
    try std.testing.expectEqual(21, packet.length);
    try std.testing.expectEqual(2021, packet.value);
    try std.testing.expectEqual(null, packet.packets);
}

test "Should parse operator packet" {
    const bits = try get_bit_stream("38006F45291200", &std.testing.allocator);
    defer std.testing.allocator.free(bits);
    const packet = Packet.parse(bits).?;

    try std.testing.expectEqual(1, packet.version);
    try std.testing.expectEqual(6, packet.typeID);
    try std.testing.expectEqual(49, packet.length);
    try std.testing.expectEqual(null, packet.value);
    try std.testing.expectEqual(2, packet.packets.?.len);

    try std.testing.expectEqual(10, packet.packets.?[0].value);
    try std.testing.expectEqual(20, packet.packets.?[1].value);
}

test "Should eval packet correctly" {
    const bits = try get_bit_stream("04005AC33890", &std.testing.allocator);
    defer std.testing.allocator.free(bits);
    const packet = Packet.parse(bits).?;

    print("{any}\n", .{packet});
    try std.testing.expectEqual(54, packet.eval());
}
