# Text2RPG
A text-based game generator

[![Gitter](https://badges.gitter.im/legendddhgf_Text2RPG/Lobby.svg)](https://gitter.im/legendddhgf_Text2RPG/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)

## Potential Ideas:
- Read in text files describing details of games in special format
- Random Room Path Generation
- Stats
- Fighting
- Multiplayer?: Server Hosting vs Database Storing
  - Issue: how do you verify that the other person is playing the same game; remember that this should be cross platform:
  - Current solution: all games function as servers, must pick a unique ID if public, game clients will be provided to connect to servers

### File Format (in regular language form):
```sh
(
    (   # Room declaration
        r $ROOM_NAME
        (d $ROOM_DESC)+
        (
            (m ($ROOM_REGEX) ($PRIORITY)?) # Declares given room(s) to be children of this room with optional priority
                                           # If a room has children, it is not "visitable", and entry will redirect the player to a 
                                           # randomly selected child weighted by priority immediately after printing the description
        |   (e $ITEM_NAME ((> | >= | < | <= | =) $VALUE)?) # Declares an expected item to be prerequesite to enter the room
                                                           # Optionally requires a specific quantity greater/less than or equal to value
                                                           # If not provided, defaults to > 0 (user has item)
        |   (a $ITEM_NAME (+ | - | =) $VALUE)              # Declares an action to inc/dec/set given item's quantity to value upon entry
        |   (   # Transitions are applied to each child room if there are children
                o $ROOM_NAME
                t $OPTION_DESC
                (e $ITEM_NAME ((> | >= | < | <= | =) $VALUE)?)* # Declares expected item to be prerequesite to perform said transition
                (a $ITEM_NAME (+ | - | =) $VALUE)*              # Declares action to inc/dec/set item's quantity to value on selection
            )
        )*
    )
|   (   # Item declaration
        i $ITEM_NAME
        (m ((+ $MAX_QTY) | (- $MIN_QTY) | (= $INIT_QTY))+ )* # Defines max/min/initial quantity, defaulting to infinity/0/0
        (d $ITEM_DESC)+
    )
)*
```

- The indentation and parenthesis are there to help with the visualization, and should not appear in actual file.
- `$ROOM_REGEX` and `$GROUP_REGEX` refer to regular expressions that match multiple `$ROOM_NAME`'s or `$GROUP_NAME`'s.
- Referencing a `$GROUP_NAME` in `o` randomly selects a member of that group with weighted `$PRIORITY`.

## Credits:
* Isaak Cherdak (@legendddhgf) - Lead Developer and Product Owner
* August Valera (@4U6U57) - Developer

### OTHERS
* Nathan Whitehead (@nwhitehead) - Designed class assignment for UCSC's CMPS 12B that inspired this project
* http://stackoverflow.com/questions/410980/include-a-text-file-in-a-c-program-as-a-char - compile game instead of game engine
