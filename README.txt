# csf-assignment3

Yoohyuk Chang, Yongjae Lee

<Contributions>
Yoohyuk Chang: Worked on the main function, the structure of the classes, cycle counts, and determining the best overall cache configuration. Fixed bugs. Worked on some load operations.
Yongjae Lee: Worked on most of the functional implementation for determining the number of loads, stores, hits, misses, etc. Worked on the structure of some classes as well.


<Determining the best overall cache configuration>
Before moving on to figuring out the best overall cache configuration, we used these following equations to calculate the overall hit rate and miss penalty.
(overall hit rate) = (total hits) / (total memory accesses)
(miss penalty) = (miss cycles) / (total misses) , where (miss cycles) = (total cycles) - (total memory accesses)


1. Lets first investigate what number of sets has the best effectiveness.
We will first just assume that the number of blocks and the number of bytes to be 4 and 16 for simplicity, respectively.
Experimenting the number of sets is important because it influences the spacial locality. The right number of sets can 
mitigate conflict misses, where multiple addresses compete for the same cache line. There is a trade-off depending on 
what number of sets I make to the cache. If we give more number of sets, it can reduce misses, but it also enlarges the 
cache, potentially slowing the access times and increasing overhead due to additional metadata.
So let's investigate on finding out the best number of sets for our cache.
Below are the experiment results. We experiment when the number of sets is equal to 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024.

./csim 1 4 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 118260
Load misses: 199937
Store hits: 100341
Store misses: 97145
Total cycles: 148738883
--> (overall hit rate) = 42.4%
--> (miss penalty) = approx. 499 cycles

./csim 2 4 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 162512
Load misses: 155685
Store hits: 121613
Store misses: 75873
Total cycles: 124236083
--> (overall hit rate) = 55.08%
--> (miss penalty) = approx. 534 cycles

./csim 4 4 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 216821
Load misses: 101376
Store hits: 151658
Store misses: 45828
Total cycles: 83945683
--> (overall hit rate) = 71.46%
--> (miss penalty) = approx. 567 cycles

./csim 8 4 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 269541
Load misses: 48656
Store hits: 175112
Store misses: 22374
Total cycles: 41926483
--> (overall hit rate) = 86.23%
--> (miss penalty) = approx. 582 cycles

./csim 16 4 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 293463
Load misses: 24734
Store hits: 182464
Store misses: 15022
Total cycles: 24405683
--> (overall hit rate) = 91.69%
--> (miss penalty) = approx. 329 cycles

./csim 32 4 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 304792
Load misses: 13405
Store hits: 185587
Store misses: 11899
Total cycles: 16394483
--> (overall hit rate) = 95.07%
--> (miss penalty) = approx. 232 cycles

./csim 64 4 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 310136
Load misses: 8061
Store hits: 186911
Store misses: 10575
Total cycles: 12757283
--> (overall hit rate) = 96.23%
--> (miss penalty) = approx. 189 cycles

./csim 128 4 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 312969
Load misses: 5228
Store hits: 187684
Store misses: 9802
Total cycles: 10701283
--> (overall hit rate) = 96.89%
--> (miss penalty) = approx. 154 cycles

./csim 256 4 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 314171
Load misses: 4026
Store hits: 188047
Store misses: 9439
Total cycles: 9685683
--> (overall hit rate) = 97.39%
--> (miss penalty) = approx. 137 cycles

./csim 512 4 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 314876
Load misses: 3321
Store hits: 188366
Store misses: 9120
Total cycles: 8786483
--> (overall hit rate) = 97.59%
--> (miss penalty) = approx. 123 cycles

./csim 1024 4 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 315334
Load misses: 2863
Store hits: 188499
Store misses: 8987
Total cycles: 7711683
--> (overall hit rate) = 97.76%
--> (miss penalty) = approx. 108 cycles

Just by looking at the results above, it seems that as we increase the number of sets, it becomes more and more effective.
The overall hit rate and miss penalty would definitely be better because we are consuming more sets. However, we should also 
take into account the overhead, latency, and the cost. We can compare this by looking at the total cycles. Notice that in the 
experiment result above that when the number of sets become 32, the overall hit rate and the miss penalty does not intensely become 
better as we increase the number of sets from there. The overall hit rate and the miss penalty difference becomes very small that from
that point (when the number of sets is 32), it should be wise to investigate more on the overhead, latency, and the cost side of analysis.
Lets look at the total cycles around when the number of sets is 32. The difference of total cycles between when number of sets is 
16 and 32 is around 8000000 and the difference between when the size is 32 and 64 is 4000000. When the number of sets if 32, the 
total cycles become more and more negligible (If we calculate the difference between the total cycles for all of the test cases above,
it will become more visiable), which indicates that it is the most effective when the number of sets is 32.

================== Most Effective Number of Sets: 32 ==================


2. Let's now determine what number of blocks in each set is the most effective. The reason for this investigation is almost the same 
as when determining the best number of sets. It influences the spacial locality (refer to 'number of sets' description above).
Below are the experiment results. We experiment when the number of blocks is equal to 1, 2, 4, 8, 16, 32, 64, 128, 256.
For simplicity, we assume the number of bytes and replacement policies to be equal for all test cases. And the number of sets will be 
32 because we already determined that 32 is the most effective.

./csim 32 1 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 243746
Load misses: 74451
Store hits: 169794
Store misses: 27692
Total cycles: 56466083
--> Overall Hit Rate: 80.32%
--> Miss Penalty: Approximately 550 cycles.

./csim 32 2 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 283673
Load misses: 34524
Store hits: 181720
Store misses: 15766
Total cycles: 29646483
--> Overall Hit Rate: 90.47%
--> Miss Penalty: Approximately 490 cycles.

./csim 32 4 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 304792
Load misses: 13405
Store hits: 185587
Store misses: 11899
Total cycles: 16394483
--> Overall Hit Rate: 95.07%
--> Miss Penalty: Approximately 232 cycles.

./csim 32 8 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 310710
Load misses: 7487
Store hits: 187034
Store misses: 10452
Total cycles: 12361683
--> Overall Hit Rate: 96.29%
--> Miss Penalty: Approximately 164 cycles.

./csim 32 16 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 313151
Load misses: 5046
Store hits: 187761
Store misses: 9725
Total cycles: 10582083
--> Overall Hit Rate: 97.16%
--> Miss Penalty: Approximately 142 cycles.

./csim 32 32 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 314293
Load misses: 3904
Store hits: 188081
Store misses: 9405
Total cycles: 9604883
--> Overall Hit Rate: 97.43%
--> Miss Penalty: Approximately 124 cycles.

./csim 32 64 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 314990
Load misses: 3207
Store hits: 188417
Store misses: 9069
Total cycles: 8728483
--> Overall Hit Rate: 97.60%
--> Miss Penalty: Approximately 115 cycles.

./csim 32 128 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 315398
Load misses: 2799
Store hits: 188517
Store misses: 8969
Total cycles: 7719283
--> Overall Hit Rate: 97.78%
--> Miss Penalty: Approximately 103 cycles.

./csim 32 256 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 315663
Load misses: 2534
Store hits: 188577
Store misses: 8909
Total cycles: 5950083
--> Overall Hit Rate: 97.86%
--> Miss Penalty: Approximately 74 cycles.

Of course, as we increase the number of blocks, the overall hit rate and the miss penalty will get better.
However, we should consider the latency problem and if the cost is expensive or not. Look at the result when 
the number of blocks is equal to 4. From that point, the overall hit rate does not greatly increase and the miss 
penalty also does not greatly decrease. So we can first assume that it is perhaps efficient when the number of 
block size is 4. Now, to investigate more accurately, let's look at the total cycle counts. The difference of 
the cycle counts from when the block size is 2 and 4 is relatively large, but the difference when the block size 
is 4 and 8 are not that large. So we may conclude that it would be the most effective overall when the number of 
blocks is equal to 4.

================== Most Effective Number of Blocks: 4 ==================

3. Let's now determine what number of bytes per block is the most effective. The reason for this is, again, 
similar to what I explained in describing the most effective number of sets. It influences the spacial locality.
(Again, refer to the 'number of sets' section for the reasoning.) Below are the experiment results.
We experiment when the number of bytes is equal to 4, 8, 16, 32, 64, 128, 256. For simplicity, we assume the 
replacement policies to be equal for all test cases to not be biased. The number of sets will be 32 and the number 
of blocks will be 4 because we already determined the most effective number of them earlier.

./csim 32 4 4 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 277454
Load misses: 40743
Store hits: 155638
Store misses: 41848
Total cycles: 11327283
--> Overall Hit Rate: 84.15%
--> Miss Penalty: Approximately 208 cycles

./csim 32 4 8 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 294518
Load misses: 23679
Store hits: 172607
Store misses: 24879
Total cycles: 14536283
--> Overall Hit Rate: 90.77%
--> Miss Penalty: Approximately 305 cycles

./csim 32 4 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 304792
Load misses: 13405
Store hits: 185587
Store misses: 11899
Total cycles: 16394483
--> Overall Hit Rate: 95.07%
--> Miss Penalty: Approximately 232 cycles

./csim 32 4 32 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 309693
Load misses: 8504
Store hits: 191425
Store misses: 6061
Total cycles: 18350083
--> Overall Hit Rate: 97.20%
--> Miss Penalty: Approximately 278 cycles

./csim 32 4 64 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 313848
Load misses: 4349
Store hits: 194405
Store misses: 3081
Total cycles: 18669283
--> Overall Hit Rate: 99.15%
--> Miss Penalty: Approximately 412 cycles

./csim 32 4 128 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 315452
Load misses: 2745
Store hits: 195718
Store misses: 1768
Total cycles: 22499683
--> Overall Hit Rate: 99.21%
--> Miss Penalty: Approximately 775 cycles

./csim 32 4 256 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 316505
Load misses: 1692
Store hits: 196529
Store misses: 957
Total cycles: 26026083
--> Overall Hit Rate: 99.59%
--> Miss Penalty: Approximately 1484 cycles

Take a look at when the byte size is 16. Up to that point, the overall hit rate greatly increases.
But when it gets to when the size is 32, the hit rate increases, but not that great.
So we can perhaps assume it is relatively effective when the byte size is 16. Also the Miss penalty 
is relatively low among all the other cases, so we can conclude that it is the most effective when 
the byte size is 16.

================== Most Effective Number of Bytes: 16 ==================

4. Let's now investigate what replacement policy combination is the most effective.
Which combination we use for the replacement policy can greatly change the efficiency of our cache.
This is because while 'write-through' ensures consistency by immediately updating main memory, it can 
increase memory traffic, whereas 'write-back' optimizes for fewer memory writes at the potential cost of 
coherence issues in multiprocessor systems. Similarly, 'write-allocate' and 'no-write-allocate' determine 
cache behavior during write misses, impacting subsequent read and write operations. Furthermore, replacement 
policies like 'LRU' and 'FIFO' can influence eviction patterns based on the application's data access behavior.
So it is very important to experiment with this. Below I set out different combinations of 'write-allocate / no-write-allocate' 
and 'write-through / write-back' and 'lru / fifo'. Since we already determined that the most effective number of 
sets to be 32, the most effective number of blocks to be 4, and the most effective number of bytes to be 16, we just 
assuem those numbers here. Also, note that here we don't have the configuration for 'no-write-allocate' and 'write-back' 
at the same time (as mentioned in the project description.)

./csim 32 4 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 304792
Load misses: 13405
Store hits: 185587
Store misses: 11899
Total cycles: 16394483
--> Overall Hit Rate: 95.07%
--> Miss Penalty: 232 cycles

./csim 32 4 16 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 307659
Load misses: 10538
Store hits: 186542
Store misses: 10944
Total cycles: 13996883
--> Overall Hit Rate: 95.83%
--> Miss Penalty: 209 cycles

./csim 32 4 16 write-allocate write-through fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 304792
Load misses: 13405
Store hits: 185587
Store misses: 11899
Total cycles: 30385883
--> Overall Hit Rate: 95.07%
--> Miss Penalty: 299 cycles

./csim 32 4 16 write-allocate write-through lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 307659
Load misses: 10538
Store hits: 186542
Store misses: 10944
Total cycles: 28857083
--> Overall Hit Rate: 95.85%
--> Miss Penalty: 274 cycles

./csim 32 4 16 no-write-allocate write-through fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 302182
Load misses: 16015
Store hits: 158619
Store misses: 38867
Total cycles: 26631416
--> Overall Hit Rate: 89.16%
--> Miss Penalty: 499 cycles

./csim 32 4 16 no-write-allocate write-through lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 305212
Load misses: 12985
Store hits: 160678
Store misses: 36808
Total cycles: 25421475
--> Overall Hit Rate: 89.74%
--> Miss Penalty: 542 cycles

From the experiment result, it is very much clear that 'write-allocate write-back lru' configuration is the most effective 
among all the other configurations. The overall hit rate is almost the same as 'write-allocate write-through lru', but 
'write-allocate write-back lru' has much less miss penalty and much less total cycles. So it is evident that 
'write-allocate write-back lru' is the best in every aspect (in terms of overall hit rate, miss penalty, and total cycles).

================== Most Effective Policy Configuration: 'write-allocate write-back lru' ==================




In conclusion, it turns out that '32 sets', '4 blocks', '16 bytes', 'write-allocate write-back lru' is the most 
effective configuration for our cache.

====================================== ** Most Effective Final Configuration ** ======================================
Number of Sets:         32
Number of Blocks:       4
Number of Bytes:        16
Policy Configuration:   write-allocate write-back lru