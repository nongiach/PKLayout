#!/usr/bin/env ruby

# 
key_map = ""
def remap(from_key, from_mod, to_key, to_mod)
  return "{{\"#{from_key}\", \"#{from_mod}\"}, {\"#{to_key}\", \"#{to_mod}\"}},"
end

# remap 'a' as shift 
puts ('a'..'z').map{ |c| remap('a', c, 'Shift_L', c) }
