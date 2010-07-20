#ifndef x_math_stats_h
#define x_math_stats_h

double x_math_stats_get_precision(unsigned long true_positives,
        unsigned long false_positives);

double x_math_stats_get_recall(unsigned long true_positives,
        unsigned long false_negatives);

double x_math_stats_get_f_measure(double precision, double recall,
        double recall_weight);

void x_math_stats_get_prf(unsigned long true_positives,
        unsigned long false_positives, unsigned long false_negatives,
        double recall_weight, double *precision, double *recall,
        double *f_measure);

#endif
