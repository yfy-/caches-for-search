Summary
========

`caches-for-search` provides various caching policies and algorithms
suited for search engine query streams. It also includes approximate
counting schemes based on bloom filters to reduce memory usage.

Included caching algorithms:

* Random
* Least Recently Used (LRU)
* Least Frequently Used (In-memory and with frequency counters)
* Greedy Dual Size Frequency (In-memory and with frequency counters)

Included frequency counters:

* Pure LFU Counter (an exact counter)
* Window LFU Counter
* Strawman Counter
* Tiny-LFU Counter

Please see our `paper
<https://link.springer.com/chapter/10.1007/978-3-030-15719-7_20>`_ for
a detailed description of the above algorithms. You can also site our
work with the below .bib entry::

  @InProceedings{10.1007/978-3-030-15719-7_20,
  author="Yafay, Erman
  and Altingovde, Ismail Sengor",
  editor="Azzopardi, Leif
  and Stein, Benno
  and Fuhr, Norbert
  and Mayr, Philipp
  and Hauff, Claudia
  and Hiemstra, Djoerd",
  title="On the Impact of Storing Query Frequency History for Search Engine Result Caching",
  booktitle="Advances in Information Retrieval",
  year="2019",
  publisher="Springer International Publishing",
  address="Cham",
  pages="155--162",
  abstract="We investigate the impact of the size of query frequency history and its compact representation in memory for search result caching.",
  isbn="978-3-030-15719-7"
  }

Example
========
Below is an example to compare the hit-rate of 3 different policies:

.. code-block:: cpp

  std::vector<std::string> queries{"a", "b", "c", "c", "a", "a", "c", "d", "a",
                                   "b", "b", "d", "a", "a", "b", "a", "a", "c"};
  auto cache_size = 3;
  LruEviction lru(cache_size);
  OnlyEvictionReplacementPolicy lru_pol(&lru, "lru");

  InMemoryLfuEviction inmem_lfu(cache_size);
  OnlyEvictionReplacementPolicy inmem_lfu_pol(&inmem_lfu, "inmem_lfu");

  // LFU policy with 1000 maximum frequency with the Tiny-LFU
  // frequency histogram.
  LfuEviction lfu(cache_size, 1000);
  TinyLfuHistogram<20, 2> tiny_fh(2, 5);
  lfu.SetFrequencyHistogram(&tiny_fh);
  OnlyEvictionReplacementPolicy lfu_pol(&lfu, "lfu");

  HitrateEvaluation eval("Cache Size", ",");
  // Evaluate policies with the cache_size and 0 warmup
  eval.Evaluate(queries, {&lru_pol, &inmem_lfu_pol, &lfu_pol}, cache_size, 0);
  std::cout << "LRU hit rate=" << eval.GetHitrate(cache_size, "lru") << "\n";
  std::cout << "In-memory LFU hit rate="
            << eval.GetHitrate(cache_size, "inmem_lfu") << "\n";
  std::cout << "LFU hit rate=" << eval.GetHitrate(cache_size, "lfu") << "\n";


Program output::

  LRU hit rate=0.666667
  In-memory LFU hit rate=0.611111
  LFU hit rate=0.722222
