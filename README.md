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
|   (   # Character declaration
        c $CHAR_NAME
        (d $CHAR_DESC)+
        (   # Inventory
            I $INVENTORY
            (
              A $ITEM_NAME
              (q ((+ $MAX_QTY | (- $MIN_QTY) | (= $INIT_QTY))+))*
              M $MAX_INV
            )

            # Power declaration
            p $POWER_NAME
            (d $POWER_DESC)+
            (
              (e $ROOM_NAME (( = ) $(PRIORITY)))
            )
        )
    )
|   (   # NPC declaration
        n $NPC_NAME
        (d $NPC_DESC)+
        (
            # Quest declaration
            Q $QUEST_NAME
            (d $QUEST_DESC)+
            (   # Requirement condition
                (a $ITEM_NAME (+ | - | =) $VALUE)
                (a $MONSTER_NAME (+ | - | =) $VALUE)
                (e $ITEM_NAME ((> | >= | =) $VALUE)?)
                (e $MONSTER__NAME ((> | >= | =) $VALUE)?)
                (a $NPC_NAME ((> | >= | =) $VALUE)?)

            |   # Reward declaration
                I $INVENTORY
                (
                  A $ITEM_NAME
                  ((q ((+ $MAX_QTY | (- $MIN_QTY) | (= $INIT_QTY))+))*)
                )
            )
        )
    )
|   (   # Monster declaration
        # TODO, merge this with NPC
        M $MONSTER_NAME
        (d $MONSTER_DEC)
        (   # Monster Drop
            A $ITEM_NAME

            #  Monster element
            A $ELEMENT_NAME

            # Power declaration
            p $POWER_NAME
            (d $POWER_DESC)+
            (
              // Can't figure out how to give monster ability to be immune,
              repel or absorb to
              ELEMENT_NAME
            )
        )
    )
|   (   # Element declaration
        # TODO
        E $ELEMENT_NAME
        Note: Here is what I want but i can't express it in expression
        E $FIRE
        (w $FIRE < $ELEMENT_NAME $ELEMENT_NAME)
        (s $FIRE > $ELEMENT_NAME $ELEMENT_NAME)
        w means fire is weak to such and such element
        s means fire is strong against such and such element
        everything else not mention will be neutral against fire
        -TESTING
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
- `I` defines an inventory for that character
- `A` adds items into the inventory
- `M` defines the maximum inventory the character can carry

Place Holder - Delete later
Alphabet Used
Capital: E, Q
Normal : a,c,d,e i, n, o, p, r, t


Unique Declarations:
- `E` declares an element with the given unique name
- `Q` declares a quest with the given unique name
- `c` declares a character with the given unique name
- `i` declares an item with the given unique name
- `n` declares a NPC with the given unique name
    - NPC can also be a monster or a boss
- `p` declares a power with the given unique name
- `r` declares a room with a given unique name

Description:
- `d` provides an optionally multi-line description of an object.
    - it describes the object given by the last `E`,`Q`, `c`, `i`, `n`, `p`, `r`

Conditions:
- `a` defines an action to be performed upon entering the room specified by the last `r`, or upon taking the transition given by the last `o` if it appears before the `t` of that transition
    - This action is defined by modifying the quantity the player has of the given item, specifically incrementing/decrementing/setting the player's quantity by the given value
    - defines an action to be performed upon receiving the quest specific given by the last `Q`
- `e` defines an entry condition that must be satisfied in order to enter the room given by the last `r`, or take the transition given by the last `o` if it appears before the `t` of that transition
    - also defines condition that must be satisfied in order to compelte the quest `Q`
    - Multiple `e` tags defined for the same entity are logically `&&`'ed (must all be met)
    - This condition bypasses the initial condition set inside `r`, resolving to `TRUE` if the player has equal the given name of the `r`
    - This condition is defined by the quantity of the given item the player has, resolving to `TRUE` if the player has less than/less than or equal/greater than/greater than or equal/exactly equal the given value of the item with the operator >/>=/</<=/=
    - also defined the quantity of monsters the player has killed, resolving to  `TRUE` if the player has the given value of monsters with the operator >=/=
    - If an entry condition is given with an item argument but no operator and  value, the comparison defaults to `> $ITEM_MIN` (checks if the player has  any amount of the item greater than the minimum amount).
    - If an entry condition is not satisfied, the initial condition set inside `r` applies.
- `q` defines quantity conditions for the item given by the last `i`
    - also for the item by the last `A`
    - These conditions set the maximum/minimum/initial value on the player's inventory for this item with the operator +/-/=, overwriting the default values of 1/0/0

TODO:
Merge Monsters with NPC
Figure out Element
Maybe merge Element as a skill
Less cluster?
Categorize them together
Figure out Health/Damage

## Credits:
- Isaak Cherdak (@legendddhgf) - Lead Developer and Product Owner
- August Valera (@4U6U57) - Developer
- Thuan Le (@thuanle123) - Developer

### OTHERS
- Nathan Whitehead (@nwhitehead) - Designed class assignment for UCSC's CMPS 12B that inspired this project
- http://stackoverflow.com/questions/410980/include-a-text-file-in-a-c-program-as-a-char - compile game instead of game engine
