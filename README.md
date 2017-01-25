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

### File Format:

Below is the specification of the game file language, in a regular language like format. Note that the indentation and parenthesis are not part of the language, they are only there to help with visualization and should not appear in the actual file. Immediately after the specification is a set of descriptions to accompany it.

```sh
(
    (   # Room declaration
        r $ROOM_NAME
        (d $ROOM_DESC)+
        (
            (m ($ROOM_REGEX) ($PRIORITY)?)
        |   (e $ITEM_NAME ((> | >= | < | <= | =) $VALUE)?)
        |   (a $ITEM_NAME (+ | - | =) $VALUE)
        |   (
                o $ROOM_NAME
				(
					(e $ITEM_NAME ((> | >= | < | <= | =) $VALUE)?)
				|	(a $ITEM_NAME (+ | - | =) $VALUE)
				)*
                t $OPTION_DESC
            )
        )*
    )
|   (   # Item declaration
        i $ITEM_NAME
        (q ((+ $MAX_QTY) | (- $MIN_QTY) | (= $INIT_QTY))+ )*
        (d $ITEM_DESC)+
    )
)*
```

The following are parts of the original specification of **@nwhitehead**, unmodified:

- `r` declares a room with a given unique name
- `d` provides an (optionally multi-line) description of the room given by the last `r`
- `o` defines a transition from the room given by the last `r` to the given room
- `t` provides a one line description for the transition given by the last `o`

Here are the modifications (actually additions) we have made to the language:

- `m` declares the given room(s) to be a children of the room given by the last `r`, with a given optional priority
    - The `$ROOM_REGEX` is a regular expression that can match one or more rooms. All rooms matched are given the same priority. If the room is already explicitly defined as a child, it is ignored by the regex.
    - The priority of the given matching children, if not provided, defaults to  100
    - A room with children is not "visitable", and conceptually can be thought of as a "group". The behavior of a transition to a group differs from that of a room.
    - A transition to a group will first print out the group's `d` as usual, and have the group's `e` and `a` tags function as normal.
    - However, immediately following that, the player will be immediately and silently (without user intervention) transitioned to a randomly selected child of the group, weighted by priority
    - Transitions of the group will be appended to the transitions of the chosen room, except for transitions that would conflict in destination with a transition already defined
- `e` defines an entry condition that must be satisfied in order to enter the room given by the last `r`, or take the transition given by the last `o` if it appears before the `t` of that transition
    - Multiple `e` tags defined for the same entity are logically `&&`'ed (must all be met)
    - This condition is defined by the quantity of the given item the player has, resolving to `TRUE` if the player has less than/less than or equal/greater than/greater than or equal/exactly equal the given value of the item with the operator >/>=/</<=/=
    - If an entry condition is given with an item argument but no operator and  value, the comparison defaults to `> $ITEM_MIN` (checks if the player has  any amount of the item greater than the minimum amount).
- `a` defines an action to be performed upon entering the room specified by the last `r`, or upon taking the transition given by the last `o` if it appears before the `t` of that transition

    - This action is defined by modifying the quantity the player has of the given item, specifically incrementing/decrementing/setting the player's quantity by the given value
- `i` declares an item with the given unique name
- `d` is modified to provide an (optionally multi-line) description of the room given by the last `r`, or alternatively the item given by the last `i`.
- `q` defines quantity conditions for the item given by the last `i`
    - These conditions set the maximum/minimum/initial value on the player's inventory for this item with the operator +/-/=, overwriting the default values of 1/0/0

## Credits:
- Isaak Cherdak (@legendddhgf) - Lead Developer and Product Owner
- August Valera (@4U6U57) - Developer

### OTHERS
- Nathan Whitehead (@nwhitehead) - Designed class assignment for UCSC's CMPS 12B that inspired this project
- http://stackoverflow.com/questions/410980/include-a-text-file-in-a-c-program-as-a-char - compile game instead of game engine
