
|             ns/find |              find/s |    err% |     total | Search for existing keys
|--------------------:|--------------------:|--------:|----------:|:-------------------------
|              200.40 |        4,990,079.41 |    2.7% |      0.49 | `map, sorted find, n = 4096`
|              148.90 |        6,716,091.88 |    3.8% |      0.37 | `map, random find, n = 4096`
|               65.30 |       15,313,525.79 |    5.4% |      0.15 | :wavy_dash: `rb, sorted find, n = 4096` (Unstable with ~199,814.4 iters. Increase `minEpochIterations` to e.g. 1998144)
|               55.32 |       18,077,907.86 |    2.8% |      0.12 | `rb, random find, n = 4096`
|               47.96 |       20,852,611.50 |    2.6% |      0.12 | `avl, sorted find, n = 4096`
|               53.56 |       18,671,653.26 |    4.8% |      0.12 | `avl, random find, n = 4096`
|              258.78 |        3,864,298.42 |    8.8% |      0.64 | :wavy_dash: `map, sorted find, n = 16384` (Unstable with ~220,067.8 iters. Increase `minEpochIterations` to e.g. 2200678)
|              276.29 |        3,619,440.66 |    3.8% |      0.67 | `map, random find, n = 16384`
|              120.46 |        8,301,200.87 |    6.8% |      0.31 | :wavy_dash: `rb, sorted find, n = 16384` (Unstable with ~220,067.8 iters. Increase `minEpochIterations` to e.g. 2200678)
|              120.41 |        8,305,082.21 |    6.9% |      0.29 | :wavy_dash: `rb, random find, n = 16384` (Unstable with ~220,067.8 iters. Increase `minEpochIterations` to e.g. 2200678)
|               95.33 |       10,489,391.93 |    6.8% |      0.22 | :wavy_dash: `avl, sorted find, n = 16384` (Unstable with ~220,067.8 iters. Increase `minEpochIterations` to e.g. 2200678)
|              104.93 |        9,529,750.31 |    2.7% |      0.22 | `avl, random find, n = 16384`
|              402.63 |        2,483,700.40 |    4.7% |      0.99 | `map, sorted find, n = 65536`
|              393.17 |        2,543,407.49 |    5.4% |      1.00 | :wavy_dash: `map, random find, n = 65536` (Unstable with ~220,067.8 iters. Increase `minEpochIterations` to e.g. 2200678)
|              161.63 |        6,187,007.29 |    4.5% |      0.40 | `rb, sorted find, n = 65536`
|              158.41 |        6,312,802.67 |    5.6% |      0.41 | :wavy_dash: `rb, random find, n = 65536` (Unstable with ~220,067.8 iters. Increase `minEpochIterations` to e.g. 2200678)
|              131.58 |        7,599,749.30 |   15.8% |      0.32 | :wavy_dash: `avl, sorted find, n = 65536` (Unstable with ~220,067.8 iters. Increase `minEpochIterations` to e.g. 2200678)
|              139.71 |        7,157,780.88 |    3.8% |      0.34 | `avl, random find, n = 65536`
|              876.46 |        1,140,950.93 |    4.8% |      2.14 | `map, sorted find, n = 262144`
|              726.04 |        1,377,332.83 |    9.3% |      1.78 | :wavy_dash: `map, random find, n = 262144` (Unstable with ~220,067.8 iters. Increase `minEpochIterations` to e.g. 2200678)
|              250.52 |        3,991,699.37 |    5.1% |      0.67 | :wavy_dash: `rb, sorted find, n = 262144` (Unstable with ~220,067.8 iters. Increase `minEpochIterations` to e.g. 2200678)
|              326.22 |        3,065,426.74 |    5.9% |      0.75 | :wavy_dash: `rb, random find, n = 262144` (Unstable with ~220,067.8 iters. Increase `minEpochIterations` to e.g. 2200678)
|              263.69 |        3,792,351.48 |    8.0% |      0.61 | :wavy_dash: `avl, sorted find, n = 262144` (Unstable with ~220,067.8 iters. Increase `minEpochIterations` to e.g. 2200678)
|              265.17 |        3,771,117.16 |   12.7% |      0.61 | :wavy_dash: `avl, random find, n = 262144` (Unstable with ~220,067.8 iters. Increase `minEpochIterations` to e.g. 2200678)

|           ns/insert |            insert/s |    err% |     total | Insert n keys into an empty structure
|--------------------:|--------------------:|--------:|----------:|:--------------------------------------
|              126.80 |        7,886,477.85 |   13.3% |      0.01 | :wavy_dash: `map, sorted insert, n = 4096` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              203.50 |        4,914,040.29 |    3.9% |      0.01 | `map, random insert, n = 4096`
|              117.33 |        8,522,715.45 |    9.8% |      0.01 | :wavy_dash: `rb, sorted insert, n = 4096` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              166.99 |        5,988,312.85 |    3.6% |      0.01 | `rb, random insert, n = 4096`
|               91.96 |       10,874,326.66 |    0.2% |      0.00 | `avl, sorted insert, n = 4096`
|              244.07 |        4,097,196.38 |    3.9% |      0.01 | `avl, random insert, n = 4096`
|              160.03 |        6,248,710.42 |    8.4% |      0.03 | :wavy_dash: `map, sorted insert, n = 16384` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              258.52 |        3,868,154.12 |    3.5% |      0.05 | `map, random insert, n = 16384`
|              158.30 |        6,316,961.57 |   14.5% |      0.03 | :wavy_dash: `rb, sorted insert, n = 16384` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              264.02 |        3,787,597.70 |    8.6% |      0.05 | :wavy_dash: `rb, random insert, n = 16384` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              115.28 |        8,674,543.15 |    1.8% |      0.02 | `avl, sorted insert, n = 16384`
|              390.56 |        2,560,457.68 |    1.5% |      0.07 | `avl, random insert, n = 16384`
|              282.02 |        3,545,891.88 |   15.0% |      0.19 | :wavy_dash: `map, sorted insert, n = 65536` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              474.46 |        2,107,669.91 |    8.6% |      0.35 | :wavy_dash: `map, random insert, n = 65536` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              257.53 |        3,882,985.02 |   11.8% |      0.19 | :wavy_dash: `rb, sorted insert, n = 65536` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              375.76 |        2,661,304.64 |    8.4% |      0.30 | :wavy_dash: `rb, random insert, n = 65536` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              138.81 |        7,204,133.23 |    5.0% |      0.14 | `avl, sorted insert, n = 65536`
|              474.38 |        2,108,024.08 |    5.0% |      0.36 | :wavy_dash: `avl, random insert, n = 65536` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              345.69 |        2,892,799.49 |   12.4% |      0.97 | :wavy_dash: `map, sorted insert, n = 262144` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              645.49 |        1,549,201.32 |    3.9% |      1.88 | `map, random insert, n = 262144`
|              328.09 |        3,047,938.92 |    6.4% |      0.86 | :wavy_dash: `rb, sorted insert, n = 262144` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              700.94 |        1,426,656.27 |    7.2% |      2.08 | :wavy_dash: `rb, random insert, n = 262144` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              176.44 |        5,667,649.84 |    6.3% |      0.85 | :wavy_dash: `avl, sorted insert, n = 262144` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              841.80 |        1,187,932.42 |    5.1% |      2.40 | :wavy_dash: `avl, random insert, n = 262144` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)

|            ns/erase |             erase/s |    err% |     total | Delete all n keys from a full structure
|--------------------:|--------------------:|--------:|----------:|:----------------------------------------
|               73.26 |       13,650,921.67 |    9.7% |      0.00 | :wavy_dash: `map, sorted erase, n = 4096` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              250.64 |        3,989,772.31 |    9.6% |      0.01 | :wavy_dash: `map, random erase, n = 4096` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|               55.86 |       17,902,332.64 |   12.4% |      0.00 | :wavy_dash: `rb, sorted erase, n = 4096` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              173.58 |        5,761,037.89 |   11.7% |      0.01 | :wavy_dash: `rb, random erase, n = 4096` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|               55.53 |       18,007,561.77 |    2.1% |      0.00 | `avl, sorted erase, n = 4096`
|              221.17 |        4,521,455.39 |    2.8% |      0.01 | `avl, random erase, n = 4096`
|               55.07 |       18,158,704.68 |   22.3% |      0.01 | :wavy_dash: `map, sorted erase, n = 16384` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              398.00 |        2,512,545.86 |   10.7% |      0.07 | :wavy_dash: `map, random erase, n = 16384` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              107.76 |        9,279,780.51 |    8.3% |      0.02 | :wavy_dash: `rb, sorted erase, n = 16384` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              228.31 |        4,379,996.80 |    7.7% |      0.04 | :wavy_dash: `rb, random erase, n = 16384` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|               67.52 |       14,811,172.02 |    5.1% |      0.01 | :wavy_dash: `avl, sorted erase, n = 16384` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              318.03 |        3,144,354.14 |    3.0% |      0.06 | `avl, random erase, n = 16384`
|              112.42 |        8,895,454.79 |   10.5% |      0.08 | :wavy_dash: `map, sorted erase, n = 65536` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              446.65 |        2,238,910.12 |   16.6% |      0.33 | :wavy_dash: `map, random erase, n = 65536` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              119.30 |        8,382,414.94 |    9.1% |      0.09 | :wavy_dash: `rb, sorted erase, n = 65536` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              379.07 |        2,638,024.84 |    8.3% |      0.27 | :wavy_dash: `rb, random erase, n = 65536` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              100.36 |        9,964,576.01 |    5.0% |      0.07 | :wavy_dash: `avl, sorted erase, n = 65536` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              501.34 |        1,994,641.90 |    4.0% |      0.36 | `avl, random erase, n = 65536`
|               84.97 |       11,768,737.91 |   34.1% |      0.27 | :wavy_dash: `map, sorted erase, n = 262144` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              857.74 |        1,165,853.98 |   10.0% |      2.37 | :wavy_dash: `map, random erase, n = 262144` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              111.93 |        8,934,300.49 |    6.2% |      0.31 | :wavy_dash: `rb, sorted erase, n = 262144` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              585.59 |        1,707,688.38 |   11.5% |      1.67 | :wavy_dash: `rb, random erase, n = 262144` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              121.11 |        8,256,801.44 |    6.6% |      0.35 | :wavy_dash: `avl, sorted erase, n = 262144` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              687.55 |        1,454,430.25 |    4.8% |      2.02 | `avl, random erase, n = 262144`
