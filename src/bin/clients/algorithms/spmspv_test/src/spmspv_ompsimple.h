#if !defined(SPMSPV_OMPSIMPLE_HEADER_)
#define SPMSPV_OMPSIMPLE_HEADER_

/*
  Operation:
  - y = alpha * A^T * x + beta * y

  Variants:
  - y dense, sparse
  - x dense, sparse
  - unit, edge weight  
*/

void
stinger_dspmTv_ompsimple (const int64_t nv,
                          const double alpha, const struct stinger *S, const double * x,
                          const double beta, double * y);

void
stinger_unit_dspmTv_ompsimple (const int64_t nv,
                               const double alpha, const struct stinger *S, const double * x,
                               const double beta, double * y);

void
stinger_dspmTspv_ompsimple (const int64_t nv,
                            const double alpha, const struct stinger *S,
                            const int64_t x_deg, const int64_t * x_idx, const double * x_val,
                            const double beta,
                            int64_t * y_deg, int64_t * y_idx, double * y_val,
                            int64_t * loc_ws, double * val_ws);

void
stinger_unit_dspmTspv_ompsimple (const int64_t nv,
                                 const double alpha, const struct stinger *S,
                                 const int64_t x_deg, const int64_t * x_idx, const double * x_val,
                                 const double beta,
                                 int64_t * y_deg, int64_t * y_idx, double * y_val,
                                 int64_t * loc_ws, double * val_ws);

#endif /* SPMSPV_OMPSIMPLE_HEADER_ */
