# 🧅 Onion tweaks

> _Little tweaks on Onion_

## FavImgFix

### Features

- Fixes the issue of images not showing up in favourites list

### Usage

- Place **FavImgFix** and **updater** in **.tmp_update** folder
- Patch applies every time you reboot

## chargingState

### Added features

- Configurable frame count & speed

### Usage

- Place **chargingState** and **chargingState.json** in **.tmp_update** folder
- Edit **chargingState.json** to change the length and speed of charging animation

  | Name        | Default Value | Description                    |
  | :---------- | :------------ | :----------------------------- |
  | frame_count | 6             | Total number of frames(max 60) |
  | frame_delay | 50000         | Delay between frames           |

#### Image file names should follow this naming convention

- chargingState<b>\#</b>.png
  > Ex. chargingState0.png, chargingState1.png

## brightnessCont

### Added features

- Digital Volume control
- Better shortcut key combinations

  | Button combination                            | Action            | Active... |
  | :-------------------------------------------- | :---------------- | :-------- |
  | <kbd>Menu</kbd> + <kbd>↑</kbd> / <kbd>↓</kbd> | Adjust Brightness | Always    |
  | <kbd>Menu</kbd> + <kbd>←</kbd> / <kbd>→</kbd> | Adjust Volume     | Always    |

### Usage

- Place **brightnessCont** in **.tmp_update** folder
