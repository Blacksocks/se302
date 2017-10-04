# User interface with asm, regs and cmd windows
define split
  layout split
  layout asm
  layout regs
  focus cmd
end

# Shortcut to change layout
define l
  layout next
end

# Restart program
define restart
  mon halt
  load
  mon reset
end

target ext :2331
mon endian little
restart
split
