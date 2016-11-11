Azinum - 2016.11.10
Abstract Syntax Tree with 2 child nodes

``` Ruby
# Node structure: [value, left, right]

[value: "Root",
    [value: "Hello",
        [NULL],
        [NULL]],
    [value: "World",
        [NULL],
        [NULL]]]


    A
   / \
  a   b
 / \
i   j

=> ast:print(3)     # tabsize: 3
"A"
---"a"
------"i"
------"j"
---"b"

=> ast:pprint(3)
"A" {
   "a" {
      "i" {},
      "j" {}
   },
   "b" {}
}
```
