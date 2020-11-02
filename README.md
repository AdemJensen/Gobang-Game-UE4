# Gobang-UE4
A gobang game developed with Unreal Engine 4.

Yes. I developed a gobang game with UE4, which sounds unnecessary because UE4 is a heavy-weight game engine.

![Main title](https://i.loli.net/2020/11/02/CyBcXxZbtlPYhfg.png)

![Play view](https://i.loli.net/2020/11/02/e6Cxm8L7dFfqicw.png)

## Build-from-source Requirements
To build this game (or even open this project in the editor), you need:
- Unreal Engine 4.24 or later.
- Plugin (Free) JSON Blueprint support 1.0
- Plugin (Free) TCP Socket Plugin 1.0
- Plugin (Paid) Simple TCP UDP Server

And also a very good computer...

## Hints
As this was my first project of UE, I used traditional TCP C/S structure rather than UE's built-in Networking system.

The project uses C++ to implement its AI core and Game logic core (Every thing involves data, such as Board Management), some of which are native C++, but all the UI-related logics, networking systems are chived via Blueprint.

Emmmm, because of the lack of knowledge at the beginning, I overused the Level Map. Actually, all the UI related and Networking related logics are in the Level Map. If you open the project, you will see looots of Bluprints and LOOOOOOOOOOOOOOTS of logics. (Don't worry, this project will be refactored, it's only a matter of time).

## Platforms
Surprisingly, thanks to the UE's cross-platform design, I am able to CORE. The system itself can support the following platforms:
- Windows
- Mac OS X
- Android
- iOS (not tested)
- Some linux with GUI (not tested)

And what's more, thanks to the `Simple TCP UDP Server` Plugin, they can play together! I once even set up a server on Android and connected it using Windows. I wonder if the built-in UE4 Networking system can do the trick.

## Copyright
Despite the spirit of sharing, this project must not be used for commercial (And I am pretty sure that no body will do that) unless with my permission. 

All the art resources in this game, I only have limited rights, such as the scene and objects, they all come from Epic Games Official Store, a PAID resource package called `Go Set`. So if you are gonna use the art resources in my game, you'd better purchase them in Epic Games store.

GPL, I assume.

## Contributing
If you trully want to contribute to this project, you can make Issues or PRs. I have noticed some of the networking related problems, but no clue. I can't even make them reappeared.