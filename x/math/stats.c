#include "cf/x/core/standard.h"
#include "cf/x/math/stats.h"

double cf_x_math_stats_get_precision(unsigned long true_positives,
    unsigned long false_positives)
{
  assert(true_positives >= 0);
  assert(false_positives >= 0);
  double denominator;
  double precision;

  denominator = (double) (true_positives + false_positives);
  if (0.0 != denominator) {
    precision = true_positives / denominator;
  } else {
    precision = 0.0;
  }

  return precision;
}

double cf_x_math_stats_get_recall(unsigned long true_positives,
    unsigned long false_negatives)
{
  assert(true_positives >= 0);
  assert(false_negatives >= 0);
  double denominator;
  double recall;

  denominator = (double) (true_positives + false_negatives);
  if (0.0 != denominator) {
    recall = true_positives / denominator;
  } else {
    recall = 0.0;
  }

  return recall;
}

double cf_x_math_stats_get_f_measure(double precision, double recall,
    double recall_weight)
{
  assert(precision >= 0.0);
  assert(recall >= 0.0);
  assert(recall_weight >= 0.0);
  double denominator;
  double weight_squared;
  double f_measure;

  weight_squared = pow(recall_weight, 2);
  denominator = ((weight_squared * precision) + recall);
  if (0.0 != denominator) {
    f_measure = ((1 + weight_squared) * (precision * recall)) / denominator;
  } else {
    f_measure = 0.0;
  }

  return f_measure;
}

void cf_x_math_stats_get_prf(unsigned long true_positives,
    unsigned long false_positives, unsigned long false_negatives,
    double recall_weight, double *precision, double *recall, double *f_measure)
{
  *precision = cf_x_math_stats_get_precision(true_positives, false_positives);
  *recall = cf_x_math_stats_get_recall(true_positives, false_negatives);
  *f_measure = cf_x_math_stats_get_f_measure(*precision, *recall,
      recall_weight);
}
