# Critical

[x] Fix exit handler or whatever I kis causing cursor to be invisible after exit
[x] Decompose render pipeline into multiple steps
[ ] Fix depth smashing issues causing pixelated edges
[ ] Implement proper pixel-space culling using Homo screen space
[ ] Implement triangle pre-culling to reduce workload

# Desired

[ ] Overhaul rendering to use barycentric coordinates (setup for below)
[ ] Implement texturing from bitmaps
[x] Implement "nice" exit (eg. pressing q or esc)
[ ] Implement user-controlled rotation
[ ] Implement other mesh types (obj is a pretty common one, and ASCII STLs _might_ be doable)
[ ] Optimize screen writes by replacing full screen dumps with single pixel updates
