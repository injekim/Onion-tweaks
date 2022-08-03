# 🧅 Onion tweaks

> _Little tweaks on Onion_

## brightnessCont

### Added features

- Digital Volume control
- Better shortcut key combinations

| Button combination                            | Action            | Active... |
| :-------------------------------------------- | :---------------- | :-------- |
| <kbd>Menu</kbd> + <kbd>↑</kbd> / <kbd>↓</kbd> | Adjust Brightness | Always    |
| <kbd>Menu</kbd> + <kbd>←</kbd> / <kbd>→</kbd> | Adjust Volume     | Always    |

### Usage

- Place **brightnessCont** file in the **.tmp_update** folder

## chargingState

### Added features

- Configurable frame count & animation speed

### Usage

- Place **chargingState** and **chargingState.json** files in the **.tmp_update** folder
- Edit **chargingState.json** to change length and speed of charging animation

  | Name        | Default Value | Description            |
  | :---------- | :------------ | :--------------------- |
  | frame_count | 6             | Total number of frames |
  | frame_delay | 50000         | Delay between frames   |

#### Image file names should follow this naming convention

- chargingState**NUMBER**.png
  > Ex. chargingState0.png, chargingState1.png
