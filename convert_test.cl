kernel void test(global uint* out32, global ulong* out64)
{
	global float *fout = (global float*)out32;
	global double *dout = (global double*)out64;

	// double to int
	out32[0] = convert_int_rte(0x0.ffffffffP0);
	out32[1] = convert_int_rtz(0x0.ffffffffP0);
	out32[2] = convert_int_rtp(0x0.ffffffffP0);
	out32[3] = convert_int_rtn(0x0.ffffffffP0);

	// double to float
	fout[4] = convert_float_rte(0x0.ffffff2P0);
	fout[5] = convert_float_rtz(0x0.ffffff2P0);
	fout[6] = convert_float_rtp(0x0.ffffff2P0);
	fout[7] = convert_float_rtn(0x0.ffffff2P0);

	// -double to int
	out32[8] = convert_int_rte(-0x0.ffffffffP0);
	out32[9] = convert_int_rtz(-0x0.ffffffffP0);
	out32[10] = convert_int_rtp(-0x0.ffffffffP0);
	out32[11] = convert_int_rtn(-0x0.ffffffffP0);

	// +-0.5 +-1.5 double to int rte
	out32[12] = convert_int_rte( 0x0.8P0);
	out32[13] = convert_int_rte( 0x1.8P0);
	out32[14] = convert_int_rte(-0x0.8P0);
	out32[15] = convert_int_rte(-0x1.8P0);

	// double to float
	fout[16] = convert_float_rte(0x0.fffffffffffff7P0);
	fout[17] = convert_float_rtz(0x0.fffffffffffff7P0);
	fout[18] = convert_float_rtp(0x0.fffffffffffff7P0);
	fout[19] = convert_float_rtn(0x0.fffffffffffff7P0);

	// long to float
	fout[20] = convert_float_rte(ULONG_MAX);
	fout[21] = convert_float_rtz(ULONG_MAX);
	fout[22] = convert_float_rtp(ULONG_MAX);
	fout[23] = convert_float_rtn(ULONG_MAX);

	// 9223371487098962000 to float
	fout[24] = convert_float_rte(9223371487098962000);
	fout[25] = convert_float_rtz(9223371487098962000);
	fout[26] = convert_float_rtp(9223371487098962000);
	fout[27] = convert_float_rtn(9223371487098962000);

	out64[0] = 0;
}
