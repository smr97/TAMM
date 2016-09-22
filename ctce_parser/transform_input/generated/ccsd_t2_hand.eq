t2 {

index h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11 = O;
index p1,p2,p3,p4,p5,p6,p7,p8,p9 = V;

array i0[V,V][O,O];
array f[N][N]: irrep_f;
array v[N,N][N,N]: irrep_v;
array t_vo[V][O]: irrep_t;
array t_vvoo[V,V][O,O]: irrep_t;
array t2_2_1[O,V][O,O];
array t2_2_2_1[O,O][O,O];
array t2_2_2_2_1[O,O][O,V];
array t2_2_4_1[O][V];
array t2_2_5_1[O,O][O,V];
array t2_4_1[O][O];
array t2_4_2_1[O][V];
array t2_5_1[V][V];
array t2_6_1[O,O][O,O];
array t2_6_2_1[O,O][O,V];
array t2_7_1[O,V][O,V];
array vt1t1_1[O,V][O,O];


t2_1:       i0[p3,p4,h1,h2] += 1 * v[p3,p4,h1,h2];
t2_2_1:     t2_2_1[h10,p3,h1,h2] += 1* v[h10,p3,h1,h2];
t2_2_2_1:   t2_2_2_1[h10,h11,h1,h2] += -1 * v[h10,h11,h1,h2];
t2_2_2_2_1: t2_2_2_2_1[h10,h11,h1,p5] += 1* v[h10,h11,h1,p5];
t2_2_2_2_2: t2_2_2_2_1[h10,h11,h1,p5] += -0.5 * t_vo[p6,h1] * v[h10,h11,p5,p6];
t2_2_2_2:   t2_2_2_1[h10,h11,h1,h2] += 1 * t_vo[p5,h1] * t2_2_2_2_1[h10,h11,h2,p5];
t2_2_2_3:   t2_2_2_1[h10,h11,h1,h2] += -0.5 * t_vvoo[p7,p8,h1,h2] * v[h10,h11,p7,p8];
t2_2_2:     t2_2_1[h10,p3,h1,h2] += 0.5 * t_vo[p3,h11] * t2_2_2_1[h10,h11,h1,h2];
t2_2_4_1:   t2_2_4_1[h10,p5] += 1* f[h10,p5];
t2_2_4_2:   t2_2_4_1[h10,p5] += -1 * t_vo[p6,h7] * v[h7,h10,p5,p6];
t2_2_4:     t2_2_1[h10,p3,h1,h2] += -1 * t_vvoo[p3,p5,h1,h2] * t2_2_4_1[h10,p5];
t2_2_5_1:   t2_2_5_1[h7,h10,h1,p9] += 1 * v[h7,h10,h1,p9];

t2_2_5_2:   t2_2_5_1[h7,h10,h1,p9] += 1 * t_vo[p5,h1] * v[h7,h10,p5,p9];
t2_2_5:     t2_2_1[h10,p3,h1,h2] += 1 * t_vvoo[p3,p9,h1,h7] * t2_2_5_1[h7,h10,h2,p9];
c2f_t2_t12a:t_vvoo[p1,p2,h3,h4] += 0.5 * t_vo[p1,h3] * t_vo[p2,h4];
t2_2_6:     t2_2_1[h10,p3,h1,h2] += 0.5 * t_vvoo[p5,p6,h1,h2] * v[h10,p3,p5,p6];
c2d_t2_t12a:t_vvoo[p1,p2,h3,h4] += -0.5 * t_vo[p1,h3] * t_vo[p2,h4];
t2_2:       i0[p3,p4,h1,h2] += -1 * t_vo[p3,h10] * t2_2_1[h10,p4,h1,h2];

lt2_3x:     i0[p3,p4,h1,h2] += -1 * t_vo[p5,h1] * v[p3,p4,h2,p5];
t2_4_1:     t2_4_1[h9,h1] += 1 *  f[h9,h1];
t2_4_2_1:   t2_4_2_1[h9,p8] += 1 * f[h9,p8];
t2_4_2_2:   t2_4_2_1[h9,p8] += 1 * t_vo[p6,h7] * v[h7,h9,p6,p8];
t2_4_2:     t2_4_1[h9,h1] += 1 * t_vo[p8,h1] * t2_4_2_1[h9,p8];
t2_4_3:     t2_4_1[h9,h1] += -1 * t_vo[p6,h7] * v[h7,h9,h1,p6];
t2_4_4:     t2_4_1[h9,h1] += -0.5 * t_vvoo[p6,p7,h1,h8] * v[h8,h9,p6,p7];
t2_4:       i0[p3,p4,h1,h2] += -1 * t_vvoo[p3,p4,h1,h9] * t2_4_1[h9,h2];
t2_5_1:     t2_5_1[p3,p5] += 1 * f[p3,p5];
t2_5_2:     t2_5_1[p3,p5] += -1 * t_vo[p6,h7] * v[h7,p3,p5,p6];
t2_5_3:     t2_5_1[p3,p5] += -0.5 * t_vvoo[p3,p6,h7,h8] * v[h7,h8,p5,p6];
t2_5:       i0[p3,p4,h1,h2] += 1 * t_vvoo[p3,p5,h1,h2] * t2_5_1[p4,p5];
t2_6_1:     t2_6_1[h9,h11,h1,h2] += -1 * v[h9,h11,h1,h2];
t2_6_2_1:   t2_6_2_1[h9,h11,h1,p8] += 1 * v[h9,h11,h1,p8];
t2_6_2_2:   t2_6_2_1[h9,h11,h1,p8] += 0.5 * t_vo[p6,h1] * v[h9,h11,p6,p8];
t2_6_2:     t2_6_1[h9,h11,h1,h2] += 1 * t_vo[p8,h1] * t2_6_2_1[h9,h11,h2,p8];
t2_6_3:     t2_6_1[h9,h11,h1,h2] += -0.5 * t_vvoo[p5,p6,h1,h2] * v[h9,h11,p5,p6];
t2_6:       i0[p3,p4,h1,h2] += -0.5 * t_vvoo[p3,p4,h9,h11] * t2_6_1[h9,h11,h1,h2];
t2_7_1:     t2_7_1[h6,p3,h1,p5] += 1 * v[h6,p3,h1,p5];
t2_7_2:     t2_7_1[h6,p3,h1,p5] += -1 * t_vo[p7,h1] * v[h6,p3,p5,p7];
t2_7_3:     t2_7_1[h6,p3,h1,p5] += -0.5 * t_vvoo[p3,p7,h1,h8] * v[h6,h8,p5,p7];
t2_7:       i0[p3,p4,h1,h2] += -1 * t_vvoo[p3,p5,h1,h6] * t2_7_1[h6,p4,h2,p5];
vt1t1_1_2:  vt1t1_1[h5,p3,h1,h2] += -2 * t_vo[p6,h1] * v[h5,p3,h2,p6];
vt1t1_1:    i0[p3,p4,h1,h2] += -0.5 * t_vo[p3,h5] * vt1t1_1[h5,p4,h1,h2];
c2f_t2_t12b:t_vvoo[p1,p2,h3,h4] += 0.5 * t_vo[p1,h3] * t_vo[p2,h4];
t2_8:       i0[p3,p4,h1,h2] += 0.5 * t_vvoo[p5,p6,h1,h2] * v[p3,p4,p5,p6];
c2d_t2_t12b:t_vvoo[p1,p2,h3,h4] += -0.5 * t_vo[p1,h3] * t_vo[p2,h4];

}
