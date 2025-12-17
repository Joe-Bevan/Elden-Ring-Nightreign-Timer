# Elden Ring Nightreign Timer
A simple overlay to help fellow nightfarers in their battle against the night.

## Quick start guide
 - Grab the latest release build (or build your own from source)
 - Run the .exe
 - Press **[ F9 ]** key to toggle **Edit mode**
 - Press **[ F10 ]** or click the physical **Start** (or **Restart** if the timer already running) buttons while in edit mode to start the timer
 - Profit ???

> [!CAUTION] 
> To close the app, you can click the 'x' on the ImGui window **while in edit mode**
> or by closing it via the taskbar at the bottom.

> [!IMPORTANT]  
> Edit mode is enabled when there is a red border around the screen. You can toggle edit mode by the [ F9 ] key 

## FAQ:
### Why do I have to manually trigger the timer?
Nightreign uses EAC (Easy Anti-Cheat) which is a kernel level anti-cheat (at least on windows). I don't have the time to dedicate to reverse engineering this and manually triggering the timer on expedition start.
This would also put yourself at greater risk of a ban. Not to mention the countless accounts I'd go through during R&D.
Just pressing F10 on the start of the day isn't that hard ;P

### Can I get banned for using this?
I take no responsibility if you do receive a ban from using this.
It does not hook into or read/write process memory to any EldenRing/FromSoft executable. 
So theoretically, it *should* be fine. Again, use at your own risk. You have been warned.