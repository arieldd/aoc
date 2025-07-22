input = File.open("input.txt")

def dividend(num, list)
  list.find { |e| num != e and e % num == 0 } || 0
end

checksum = 0
sum = 0
input.each do |row|
  values = row.split.map { |v| v.to_i }

  checksum += values.max - values.min
  values.each { |v| sum += dividend(v, values) / v }
end

puts(checksum)
puts(sum)
