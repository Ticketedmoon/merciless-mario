## Merciless Mario

### What is this project?
A c++ game where the player plays as the classic NES 1983 protagonist with a few malevolent twists.

### Controls

**All Scenes**
- Push `escape` to return to the previous scene, or if on the menu scene, exit the game.

**Menu Scene**
- Select menu item with `mouse-left`
- Or move up and down menu items with `arrow-up` or `arrow-down`
  - Select current menu item with `enter`

**Gameplay Scene**
- Move around with `W, S, A, D` or the arrow keys.
- `space` to jump.
- `left-click` to shoot with your mouse.
- `right-click` to reload.

---

### Tools
- C++
- SFML
- Piskel (Spritesheet generation)

### Dev Log 

Date: `08/10/2023`

- Simple Movable player entity.
- Collision with other game entities (Platforms)
- Jump mechanic (`Space` key)
- Basic gravity (Constant)
- Basic acceleration and deceleration 

![Sun 08 Oct 2023 18:32:55 IST](https://github.com/Ticketedmoon/merciless-mario/assets/21260839/4543b6a0-b51f-4a4b-8daa-13264f6816ef)

---

Date: `03/11/2023`

- Character sprite and animations
- Block sprites and animations
- Mario jump sfx
- Block interaction sfx (Question Block, Power-Up, Bump, Brick Break)
- Gun aiming and firing projectiles with the mouse (point-and-click).

![Fri 03 Nov 2023 22:21:01 GMT](https://github.com/Ticketedmoon/merciless-mario/assets/21260839/da0ba7cd-6adb-4592-91a9-342c239b7ad0)

---

Date: `04/11/2023`

- Level restructured to look more like mario 1-1
- Added background decorations
- Added pipes

![Sat 04 Nov 2023 17:11:26 GMT](https://github.com/Ticketedmoon/merciless-mario/assets/21260839/e765199d-c603-4129-b9a4-ea8bcdcbe849)

---

![Sat 04 Nov 2023 17:11:38 GMT](https://github.com/Ticketedmoon/merciless-mario/assets/21260839/80875122-76e6-4f8c-b7dc-2c79e105c51e)

---

![Sat 04 Nov 2023 17:11:53 GMT](https://github.com/Ticketedmoon/merciless-mario/assets/21260839/d1c5dc3d-4a1a-4ad1-bdb3-5971ac6fdfee)

---

Date: `07/11/2023`

- Updated Mario's appearance to wear sunglasses (Because he's merciless!)
- Added Enemy Type: `Goombas`
  - Goombas have movement and collision properties.
  - Goombas can kill the player on touch.
  - Goombas will display a death animation for a period of time when jumped on before being destroyed. 
  - Included the classic 'stomp' sound effect.

![Tue 07 Nov 2023 17:35:22 GMT](https://github.com/Ticketedmoon/merciless-mario/assets/21260839/4683d621-bdf1-4f57-bf29-cba4d7514a1a)

---

Date: `07/11/2023`

- Added Ammunition System, where the player must reload upon firing of all bullets in the current magazine.
- Added gun fire and reload sfx.
- Added ammo text rendering into the UI (temporary) for resource visibility.

![Tue 07 Nov 2023 20:48:03 GMT](https://github.com/Ticketedmoon/merciless-mario/assets/21260839/56a5335d-fe5b-4360-a6af-7c3965c4edf9)

---

Date: `09/11/2023`

- Fixed bug where weapon spawning was relative to the player position, not the question block.
- Fixed bug with the weapon rotation when mouse is behind the player.

![Sat 11 Nov 2023 23:00:38 GMT](https://github.com/Ticketedmoon/merciless-mario/assets/21260839/bc276ccd-207f-4011-9b1c-c7acbc841dd1)

Date: `09/11/2023`

- Added main menu / level select scene

![Sun 12 Nov 2023 00:45:17 GMT](https://github.com/Ticketedmoon/merciless-mario/assets/21260839/5ceccd8e-dffd-4fe6-ac66-90ea5b5133dd)

---