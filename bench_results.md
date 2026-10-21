
|             ns/find |              find/s |    err% |     total | Search for existing keys
|--------------------:|--------------------:|--------:|----------:|:-------------------------
|              208.23 |        4,802,416.87 |    2.0% |      0.50 | `map, sorted find, n = 4096`
|              163.89 |        6,101,524.88 |    4.4% |      0.41 | `map, random find, n = 4096`
|               67.55 |       14,804,417.50 |    7.4% |      0.15 | :wavy_dash: `rb, sorted find, n = 4096` (Unstable with ~199,814.4 iters. Increase `minEpochIterations` to e.g. 1998144)
|               60.41 |       16,554,662.02 |    2.4% |      0.13 | `rb, random find, n = 4096`
|               46.56 |       21,476,423.93 |    4.5% |      0.12 | `avl, sorted find, n = 4096`
|               58.15 |       17,195,890.56 |    2.1% |      0.13 | `avl, random find, n = 4096`
|              249.56 |        4,006,999.29 |    3.9% |      0.62 | `map, sorted find, n = 16384`
|              243.38 |        4,108,838.36 |    3.7% |      0.58 | `map, random find, n = 16384`
|              121.67 |        8,218,687.98 |    3.9% |      0.30 | `rb, sorted find, n = 16384`
|              111.11 |        8,999,762.63 |    4.5% |      0.27 | `rb, random find, n = 16384`
|               86.19 |       11,602,165.92 |    5.0% |      0.18 | `avl, sorted find, n = 16384`
|               98.24 |       10,179,391.41 |    1.8% |      0.22 | `avl, random find, n = 16384`
|              353.62 |        2,827,931.59 |    1.0% |      0.88 | `map, sorted find, n = 65536`
|              376.26 |        2,657,737.72 |    6.2% |      0.88 | :wavy_dash: `map, random find, n = 65536` (Unstable with ~220,067.8 iters. Increase `minEpochIterations` to e.g. 2200678)
|              179.88 |        5,559,360.21 |   10.5% |      0.44 | :wavy_dash: `rb, sorted find, n = 65536` (Unstable with ~220,067.8 iters. Increase `minEpochIterations` to e.g. 2200678)
|              192.32 |        5,199,544.63 |    3.2% |      0.46 | `rb, random find, n = 65536`
|              152.03 |        6,577,561.61 |   12.4% |      0.37 | :wavy_dash: `avl, sorted find, n = 65536` (Unstable with ~220,067.8 iters. Increase `minEpochIterations` to e.g. 2200678)
|              189.17 |        5,286,153.60 |    8.4% |      0.45 | :wavy_dash: `avl, random find, n = 65536` (Unstable with ~220,067.8 iters. Increase `minEpochIterations` to e.g. 2200678)
|              938.59 |        1,065,423.84 |    2.5% |      2.21 | `map, sorted find, n = 262144`
|              906.28 |        1,103,411.22 |   12.1% |      2.22 | :wavy_dash: `map, random find, n = 262144` (Unstable with ~220,067.8 iters. Increase `minEpochIterations` to e.g. 2200678)
|              351.12 |        2,848,047.05 |   12.8% |      0.88 | :wavy_dash: `rb, sorted find, n = 262144` (Unstable with ~220,067.8 iters. Increase `minEpochIterations` to e.g. 2200678)
|              334.77 |        2,987,097.86 |    9.0% |      0.78 | :wavy_dash: `rb, random find, n = 262144` (Unstable with ~220,067.8 iters. Increase `minEpochIterations` to e.g. 2200678)
|              322.75 |        3,098,360.19 |    1.0% |      0.76 | `avl, sorted find, n = 262144`
|              328.43 |        3,044,817.27 |   10.6% |      0.79 | :wavy_dash: `avl, random find, n = 262144` (Unstable with ~220,067.8 iters. Increase `minEpochIterations` to e.g. 2200678)

|           ns/insert |            insert/s |    err% |     total | Insert n keys into an empty structure
|--------------------:|--------------------:|--------:|----------:|:--------------------------------------
|              211.75 |        4,722,582.85 |    4.8% |      0.01 | `map, sorted insert, n = 4096`
|              356.48 |        2,805,237.36 |   14.3% |      0.02 | :wavy_dash: `map, random insert, n = 4096` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              102.90 |        9,718,435.84 |    9.2% |      0.00 | :wavy_dash: `rb, sorted insert, n = 4096` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              169.70 |        5,892,880.78 |    7.7% |      0.01 | :wavy_dash: `rb, random insert, n = 4096` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|               95.42 |       10,480,233.14 |    7.0% |      0.00 | :wavy_dash: `avl, sorted insert, n = 4096` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              258.65 |        3,866,178.86 |    1.4% |      0.01 | `avl, random insert, n = 4096`
|              170.14 |        5,877,571.17 |    1.8% |      0.03 | `map, sorted insert, n = 16384`
|              259.19 |        3,858,211.66 |   23.7% |      0.04 | :wavy_dash: `map, random insert, n = 16384` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              213.69 |        4,679,638.69 |    9.6% |      0.04 | :wavy_dash: `rb, sorted insert, n = 16384` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              291.65 |        3,428,759.18 |   16.0% |      0.05 | :wavy_dash: `rb, random insert, n = 16384` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              141.19 |        7,082,807.55 |   11.0% |      0.02 | :wavy_dash: `avl, sorted insert, n = 16384` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              423.78 |        2,359,737.55 |   10.9% |      0.07 | :wavy_dash: `avl, random insert, n = 16384` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              270.29 |        3,699,691.37 |    7.3% |      0.18 | :wavy_dash: `map, sorted insert, n = 65536` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              355.46 |        2,813,226.06 |    1.7% |      0.26 | `map, random insert, n = 65536`
|              221.69 |        4,510,774.45 |   14.5% |      0.16 | :wavy_dash: `rb, sorted insert, n = 65536` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              373.46 |        2,677,667.48 |   11.2% |      0.26 | :wavy_dash: `rb, random insert, n = 65536` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              213.68 |        4,679,975.54 |    9.3% |      0.16 | :wavy_dash: `avl, sorted insert, n = 65536` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              594.67 |        1,681,609.93 |    4.9% |      0.42 | `avl, random insert, n = 65536`
|              381.49 |        2,621,278.58 |    2.0% |      1.02 | `map, sorted insert, n = 262144`
|              568.85 |        1,757,925.78 |    5.8% |      1.64 | :wavy_dash: `map, random insert, n = 262144` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              266.21 |        3,756,412.07 |    2.3% |      0.76 | `rb, sorted insert, n = 262144`
|              618.31 |        1,617,320.20 |   13.9% |      1.77 | :wavy_dash: `rb, random insert, n = 262144` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              317.79 |        3,146,697.62 |    4.6% |      0.97 | `avl, sorted insert, n = 262144`
|              827.26 |        1,208,815.49 |    5.7% |      2.45 | :wavy_dash: `avl, random insert, n = 262144` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)

|            ns/erase |             erase/s |    err% |     total | Delete all n keys from a full structure
|--------------------:|--------------------:|--------:|----------:|:----------------------------------------
|               88.41 |       11,310,480.42 |    9.1% |      0.00 | :wavy_dash: `map, sorted erase, n = 4096` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              241.33 |        4,143,740.03 |    5.4% |      0.01 | :wavy_dash: `map, random erase, n = 4096` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|               48.13 |       20,777,850.49 |    6.2% |      0.00 | :wavy_dash: `rb, sorted erase, n = 4096` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              155.21 |        6,442,743.58 |    3.6% |      0.01 | `rb, random erase, n = 4096`
|               65.09 |       15,363,610.45 |   11.9% |      0.00 | :wavy_dash: `avl, sorted erase, n = 4096` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              224.77 |        4,448,924.26 |    3.2% |      0.01 | `avl, random erase, n = 4096`
|               79.10 |       12,642,385.02 |    6.5% |      0.01 | :wavy_dash: `map, sorted erase, n = 16384` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              310.36 |        3,222,102.87 |    1.6% |      0.06 | `map, random erase, n = 16384`
|               70.43 |       14,198,422.61 |    4.3% |      0.01 | `rb, sorted erase, n = 16384`
|              224.54 |        4,453,554.76 |    3.5% |      0.04 | `rb, random erase, n = 16384`
|               77.77 |       12,859,222.75 |    3.7% |      0.01 | `avl, sorted erase, n = 16384`
|              343.95 |        2,907,396.65 |    1.7% |      0.06 | `avl, random erase, n = 16384`
|              125.01 |        7,999,389.70 |    3.9% |      0.09 | `map, sorted erase, n = 65536`
|              461.14 |        2,168,519.11 |    4.5% |      0.35 | `map, random erase, n = 65536`
|              117.53 |        8,508,144.57 |   20.5% |      0.08 | :wavy_dash: `rb, sorted erase, n = 65536` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              452.35 |        2,210,669.86 |    2.7% |      0.33 | `rb, random erase, n = 65536`
|              117.66 |        8,499,186.08 |   11.1% |      0.09 | :wavy_dash: `avl, sorted erase, n = 65536` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              508.83 |        1,965,285.61 |    3.8% |      0.38 | `avl, random erase, n = 65536`
|              153.72 |        6,505,433.61 |    4.0% |      0.44 | `map, sorted erase, n = 262144`
|              778.17 |        1,285,059.84 |    6.9% |      2.25 | :wavy_dash: `map, random erase, n = 262144` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              114.88 |        8,704,931.56 |   16.4% |      0.33 | :wavy_dash: `rb, sorted erase, n = 262144` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              339.50 |        2,945,528.45 |    7.6% |      0.98 | :wavy_dash: `rb, random erase, n = 262144` (Unstable with ~1.0 iters. Increase `minEpochIterations` to e.g. 10)
|              112.40 |        8,897,085.70 |    3.4% |      0.33 | `avl, sorted erase, n = 262144`
|              306.24 |        3,265,395.20 |    2.6% |      0.90 | `avl, random erase, n = 262144`
