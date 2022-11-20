# My everyday Window Manager (DWM) :heart:

This window manager keep the philosophy of `unix`

## Do one thing and do it well

~~~bash
Linux Use		:- If there is terminal there is a way.
Developer User	:- If there is vim there is geek.

~~~

# Basic Reference (Navigation)

- [Mod]					- Window Kye (Microsoft) / Command Kye(Mac Os)
- [Mod]+[Tag]				- To focus on another tag
- [Shift]+[Mod]+[Tag]		- move active window to the 2 tag.
- [Shift]+[Mod]+[Enter]   - launch terminal.
- [Mod]+[b]               - show/hide bar.
- [Mod]+[p]               - dmenu for running programs like the x-www-browser.
- [Mod]+[Enter]           - push acive window from stack to master, or pulls last used window from stack onto master.
- [Mod] + [j / k]         - focus on next/previous window in current tag.
- [Mod] + [h / l]         - increases / decreases master size
- [Mod] + [i / d]         - To change the amount of windows in the master area
- [Mod]+[Return]			- To toggle a window between the master and stack area, press.
- [Shift]+[Mod]+[c]		- To kill a window
- [Mod]+[x]				- To kill a window. ( Right click discard) - User define
- [Mod]+[Shift]+[space]	- will toggle floating mode on for active window. (Very useful without disturbing current mode)
- [Mod]+[Space]           - toggles to the previous layout mode.


- [Mod]+[0]               - view all windows on screen.
- [Shift]+[Mod]+[0]       - make focused window appear on all tags.

- [Mod]+[Ctrl]+[Tag] 		- Import all window from multiple tag number( Press it again to send it back)
- [Mod]+[Ctrl]+[Shift][Tag]-Export current window to multiple tag number ( Press it again to bright it back) 

# Using Mouse
- [Left click ] + [Tag Number ]		- Jumpt to tag
- [Right click ] + [Over Tag Number ] - Bring all windows of that tag into current
- [Mod]+[right mouse button]			- to resize the floating window
- [Mod]+[left mouse button]			- to move it around.
- [Mod]+[middle mouse button]			- onto the window, to toggle being floating (Very useful without disturbing current mode )

# Layout
By default dwm is in tiled layout mode.
- [Mod]+[t]               - Tiled mode.
- [Mod]+[f]               - floating mode.
- [Mod]+[m]               - monocle mode.
- [Mod]+[c]				- Center master mode.
- [CTRL]+[Mod]+[c]		- Center floating master mode.
- [Mod]+[g]				- Grid mode.

# Screens
I am using multiple monitor and it's working well.
- [Mod]+[,]				- focus previous screen
- [Mod}+[.]				- focus next screen
- [Mod]+[Shift]+[,]		- send window to previous screen
- [Mod]+[Shift]+[.]		- send window to next screen

# Restarting/Quitting
- [Shift]+[Mod]+[q]		- Reload dwm with latest configuration and settings.
- For quiting/restart of system, I would prefer to `init 0/6` or `shutdown now` (without any delay)

# Status
- By default dwm is showing dwm-X.X in its statusbar. This text can be changed by setting the WM_NAME property of the root window.
- For simplicity I would prefer to work with `conky` see conky and it's configuration


# Patches
For my own preference , I have added few patches which suite my working style.

- By default dwm is showing dwm-X.X in its statusbar. This text can be changed by setting the WM_NAME property of the root window.
- For simplicity I would prefer to work with `conky` see conky and it's configuration
- My custom patch by my self at [here](https://github.com/vrkansagara/dwm/blob/master/dwm.c#L721)

# Defining tags and workspaces ( I like this )
Users often confuse dwm's tagging system with workspaces (or virtual desktops). Once you understand that tags are not workspaces, you'll realise why dwm was given its name - dynamic window manager - and how powerful it is.

A tag is simply a label placed on windows. A window may have one or more tags. Tags provide no other functionality.

# Where to gro from here
- Suckless Terminal `st` ( https://github.com/vrkansagara/st )
- Suckless Slock `slock` ( https://github.com/vrkansagara/slock) 
- My IDE using ( VIM + UNIX)  (https://github.com/vrkansagara/ide)

# TODO
- [ ] Add support natural scrolling support for the touchapad,mouse
- [ ] Add notification( i.e. skype,teams message pop up on screen), help on
  bettery notification
- [ ] Add sound card(mic and volume) change/select support for the system 
- [ ] Add dual clock into dmenu.
- [ ] Add screen lock support
- [ ] Toggle auto screen brightness 
- [ ] Add support for xclip with hotkey
- [ ] Check battery health using command line.
- [ ] Show/Hde fan speed or sensor list with its low/high normal temperature
- [ ] Implemente Scrot for the screen capture.
- [ ] Secure ssh agent ( ref. https://rabexc.org/posts/pitfalls-of-ssh-agents)
- [ ] if power cable attached then change the icon. in dmenu.


# References
- Ref:- https://gist.github.com/erlendaakre/12eb90eef84a3ab81f7b531e516c9594
- https://ratfactor.com/dwm
- https://github.com/w0ng/wongdev.com/blob/master/content/dwm-tags-are-not-workspaces.md
