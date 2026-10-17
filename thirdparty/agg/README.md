# Anti-Grain Geometry 2.4

The parts of AGG 2.4 that Ramen uses, from
https://github.com/rougier/antigrain-2.4 (commit f0770e311bd1ea0ea2fb814eaf0820f15ea55ff7).

AGG 2.4 is distributed under the Modified BSD license or the Anti-Grain
Geometry Public License, see LICENSE.txt. Later versions (2.5) are GPL, which is
why this copy is used instead of the system package.

Changes from the original:

- Headers moved to `include/agg2`, matching the system packages layout.
- Removed the `register` keyword, which is not valid in C++17.
