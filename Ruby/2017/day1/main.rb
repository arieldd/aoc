input = File.open("input.txt")

def captcha(text, step)
  last = text.size - 1

  sum = 0
  for i in 0..last
    j = (i + step) % last
    d1 = text[i].to_i
    d2 = text[j].to_i
    sum += d1 == d2 ? d1 : 0
  end

  sum
end

input.each do |line|
  puts(captcha(line, 1))
  puts(captcha(line, line.size / 2))
end
