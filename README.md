# Elden Ring Nightreign Timer
A simple overlay to help fellow nightfarers in their battle against the night.

## Quick start guide
 - Grab the latest release build (or build your own from source)
 - Run the .exe
 - Press **[ F9 ]** key to toggle **Edit mode**
   - While in edit mode you can drag and move the overlay to a more suitable location
 - Press **[ F10 ]** or click the physical **Start** (or **Restart** if the timer already running) buttons while in edit mode to start the timer
 - Profit ???

> [!CAUTION] 
> To close the app, you can click the 'x' on the ImGui window **while in edit mode**
> or by closing it via the taskbar at the bottom.
> 
> While in edit mode, inputs will not be passed through to the application underneath. Remeber to exit edit mode before playing.

> [!IMPORTANT]  
> Edit mode is enabled when there is a red border around the screen. You can toggle edit mode by the [ F9 ] key

> [!NOTE]
> After the first night, you need to restart the timer when the second day stats.
> 
> For the best success start/restate the timer when you first see the "Day 1/2" image show up on your screen.

## Photos and notes
There are two main components to the timer.
- The time left until the current ring starts/stops closing
- How far through the day you are (also shown in progress bar format)

The countdown will be green when you're in a 'free roam' phase where the ring isnt moving in.
The timer will be red when the ring is closing in.



## Timer in progress
![Timer](https://github.com/Joe-Bevan/Elden-Ring-Nightreign-Timer/blob/main/res/Normal.png)

## Timer in edit mode (before starting a run)
![Timer in edit mode](https://github.com/Joe-Bevan/Elden-Ring-Nightreign-Timer/blob/main/res/EditMode.png)

## Timer in game
![Timer in game](https://github.com/Joe-Bevan/Elden-Ring-Nightreign-Timer/blob/main/res/InGame.png)

## FAQ:
### Why do I have to manually trigger the timer?
Nightreign uses EAC (Easy Anti-Cheat) which is a kernel level anti-cheat (at least on windows). I don't have the time to dedicate to reverse engineering this and manually triggering the timer on expedition start.
This would also put yourself at greater risk of a ban. Not to mention the countless accounts I'd go through during R&D.
Just pressing F10 on the start of the day isn't that hard ;P

### Can I get banned for using this?
I take no responsibility if you do receive a ban from using this.
It does not hook into or read/write process memory to any EldenRing/FromSoft executable. 
So theoretically, it *should* be fine. Again, use at your own risk. You have been warned.
