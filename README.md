# K-neighbors-project
## Αργύριος Λαζαρίδης  - 1115202100083 (sdi2100083@di.uoa.gr)
## Σταύρος Πρέντζας    - 1115202100164 (sdi2100164@di.uoa.gr) 
## Στέφανος Φωτόπουλος - 1115202100209 (sdi2100di.uoa.gr)


### Κληση main

Ορίσματα
Flags:     dataset_name(-f), 
            k (-k), 
            L (-L), 
            R (-R), 
            a (-a),                 (default 1.2)
            vamana_type (-v)        ("simple", "filtered" , "stitched")
            Rstitched (-Rst) 


### Εντολες Make

- **make:**             Δημιουργια εκτελέσιμων test και main
- **make create:**      Δημιουργία directories /bin/, /lib/ ... 
- **make run:**         Εκτέλεση simple Vamana με παραμέτρους -f small -k 100 -L 120 -R 30 -a 1.2 -v simple
- **make run_test:**    Εκτέλεση test 
- **make clean:**       Διαγραφή αρχείων σε φακέλους bin & build (διατήρηση αρχείων _graph.bin & _medoid_map_bin)
- **make erase:**       Διαγραφή αρχείων σε φακέλους bin & build (διαγραφή και των αρχείων _graph.bin & _medoid_map_bin)
Τα arguments βρίσκονται και αλλάζουν μέσα στο Makefile
- **make run_simple:**      Compile + time ./bin/main  -f dummy -k 100 -L 120 -R 30 -a 1.2 -v filtered 
- **make run_filtered:**    time ./bin/main  -f dummy -k 100 -L 150 -R 42 -a 1.2 -v filtered 
- **make run_stitched:**    time ./bin/main  -f dummy -k 100 -L 150 -R 42 -a 1.2 -v stitched -Rst 96
- **make valgrind:**        Εκτέλεση test με valgrind


- Ο Στέφανος Φωτόπουλος (1115202100209) ασχολήθηκε με την αρχική σχεδίαση και υλοποίηση του graph.hpp, την υλοποίηση βοηθητικών συναρτήσεων στο utils.h (rDirectional, medoid, retain_closest_points, Euclidean_distance, find_min_euclidean) και τα τεστ των (rDirectional medoid). Επίσης έκανε την υλοποίηση του Greedy αλγόριθμου στο αρχείο knn.hpp.

- Ο Αργύριος Λαζαρίδης (1115202100083) ασχολήθηκε με την αρχική σχεδίαση όλων των modules και υλοποίησε τα περισσότερα unit test (εκτός αυτών που αναφέρονται από τα άλλα μέλη) όπως και των αλγορίθμων του knn.hpp για τα οποία χρειάστηκε η λύση τους στο χαρτί προκειμένου να γίνει επαλήθευση των αποτελεσμάτων. Εκτός αυτού υλοποίησε τις αρχικες μορφές των συναρτήσεων vec_to_graph και make_vec.

- Ο Σταύρος Πρέντζας (1115202100164) ασχολήθηκε με την αρχική σχεδίαση, την προσαρμογή της vec_to_graph, την υλοποίηση των αλγορίθμων RobustPrune και Vamana, την υλοποίηση του τεστ της RobustPrune και την επέκταση του τεστ του Vamana για float vectors.


Όλα τα μέλη της ομάδας λειτούργησαν συνεργατικά, βοηθώντας ο ένας τον άλλον στο debugging, στην σχεδίαση ή στην κατανόηση μερών της εργασίας. Αρκετά συχνά γινόταν χρήση LiveShare παράλληλα με βιντεοκλήση, ώστε να υπάρχει δυνατότητα πιο άμεσης επικοινωνίας και πιο αποτελεσματικής γραφής κώδικα. Έτσι για παράδειγμα δημιουργήθηκε το main αρχείο με συμμετοχή όλων των μελών της ομάδας.
