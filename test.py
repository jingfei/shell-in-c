#usr/bin/env python2

import curses
import time

# Create curses screen
scr = curses.initscr()
scr.keypad(True)
scr.refresh()
curses.noecho()

# Get screen width/height
height,width = scr.getmaxyx()

# Create a curses pad (pad size is height + 10)
mypad_height = height + 10
mypad = curses.newpad(mypad_height, width);
mypad.scrollok(True)
mypad_pos = 0
mypad_refresh = lambda: mypad.refresh(mypad_pos, 0, 0, 0, height-1, width)
mypad_refresh()

# Fill the window with text (note that 5 lines are lost forever)
for i in range(0, height + 15):
    mypad.addstr("{0} This is a sample string...\n".format(i))
    if i > height: mypad_pos = min(i - height, mypad_height - height)
    mypad_refresh()
    time.sleep(0.05)

# Wait for user to scroll or quit
running = True
while running:
    ch = scr.getch()
    if ch == curses.KEY_DOWN and mypad_pos < mypad_height - height:
        mypad_pos += 1
        mypad_refresh()
    elif ch == curses.KEY_UP and mypad_pos > 0:
        mypad_pos -= 1
        mypad_refresh()
    elif ch < 256 and chr(ch) == 'q':
        running = False

# Store the current contents of pad
mypad_contents = []
for i in range(0, mypad_height):
    mypad_contents.append(mypad.instr(i, 0))

# End curses
curses.endwin()

# Write the old contents of pad to console
print '\n'.join(mypad_contents)
