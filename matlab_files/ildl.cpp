#include <mex.h>
#include <string>
#include "../source/solver.h"
#include "utils.h"

using namespace std;

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
	// validate number of inputs and outputs
	// validate number of inputs and outputs
	if (nrhs < 1)
		mexErrMsgTxt("Not enough input arguments.");
	if (nrhs > 7)
		mexErrMsgTxt("Too many input arguments.");
	if (nlhs > 5)
		mexErrMsgTxt("Too many output arguments.");
	if (!mxIsSparse(prhs[0])) 
		mexErrMsgTxt("Input matrix must be sparse. Try wrapping the input with the sparse() function.");
	
	mex_utils m;
    
	// set up raw variables
	const mxArray* raw_csc = prhs[0];
	if (mxGetN(raw_csc) != mxGetM(raw_csc))
		mexErrMsgTxt("matrix must be square.");
	
	double fill_factor = 3.0, tol = 0.001, pp_tol = 1.0;
	if (nrhs > 1) {
		const mxArray* raw_fill_fact = prhs[1];
		fill_factor = m.parse_double(raw_fill_fact);
	}
	if (nrhs > 2) {
		const mxArray* raw_tol = prhs[2];
		tol = m.parse_double(raw_tol);
	}
	if (nrhs > 3) {
		const mxArray* raw_ordering = prhs[3];
		char* ordering = m.parse_str(raw_ordering);

		m.solv.set_reorder_scheme(ordering);
	}
	if (nrhs > 4) {
		const mxArray* raw_equil = prhs[4];
		char* equil = m.parse_str(raw_equil);
		m.solv.set_equil(equil);
	}
    if (nrhs > 5) {
		const mxArray* raw_piv_type = prhs[5];
		char* piv_type = m.parse_str(raw_piv_type);
        m.solv.set_pivot(piv_type);
	}
	if (nrhs > 6) {
		const mxArray* raw_pp_tol = prhs[6];
		pp_tol = m.parse_double(raw_pp_tol);
	}

	//--------------- load A ---------------//
	double* m_x;
	mwSize* m_row_idx;
	mwSize* m_col_idx;
	mwSize nnzs;

	m_x = mxGetPr(raw_csc);
	m_col_idx = mxGetJc(raw_csc);
	m_row_idx = mxGetIr(raw_csc);
	nnzs = mxGetN(raw_csc);

	m.mex_convert(m_x, m_col_idx, m_row_idx, nnzs);
	//--------------------------------------//

	//factor A.
	m.solv.solve(fill_factor, tol, pp_tol, -1);

	m.mex_save_lhs(plhs[0], 'L');
	m.mex_save_lhs(plhs[1], 'D');
	m.mex_save_lhs(plhs[2], 'P');
	m.mex_save_lhs(plhs[3], 'S');

	if (nlhs == 5)
		m.mex_save_lhs(plhs[4], 'A');

}
