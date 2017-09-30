target ext :2331
mon endian little
mon halt
load
mon reset

# User interface with asm, regs and cmd windows
define split
  layout split
  layout asm
  layout regs
  focus cmd
end

split

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
