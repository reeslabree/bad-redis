# bad-redis

bad-redis is a tiny, educational, in-memory key–value store written in C with a simple text-based TCP protocol. It’s intentionally minimal so the internals (hashing, locking, parsing, networking) are easy to reason about. Written as an exercise to reacquaint myself with C.

#### Protocol

Plain text over TCP, one command per line.

```
PUT my_key "hello world"
=> OK

GET my_key
=> "hello world"

DELETE my_key
=> OK

GET missing_key
=> NOT_FOUND
```
