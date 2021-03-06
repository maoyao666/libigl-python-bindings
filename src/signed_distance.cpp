#include <common.h>
#include <npe.h>
#include <typedefs.h>


#include <igl/signed_distance.h>

const char *ds_signed_distance = R"igl_Qu8mg5v7(
SIGNED_DISTANCE computes signed distance to a mesh


Parameters
----------
     P  #P by 3 list of query point positions
     V  #V by 3 list of vertex positions
     F  #F by ss list of triangle indices, ss should be 3 unless sign_type
     return_normals  (Optional, defaults to False) If set to True, will return pseudonormals of
                     closest points to each query point in P
Returns
-------
    S  #P list of smallest signed distances
    I  #P list of facet indices corresponding to smallest distances
    C  #P by 3 list of closest points

See also
--------


Notes
-----
    Known issue: This only computes distances to triangles. So unreferenced
    vertices and degenerate triangles are ignored.

Examples
--------
>>> S, I, C = signed_distance(P, V, F, return_normals=False)

)igl_Qu8mg5v7";

npe_function(signed_distance)
npe_doc(ds_signed_distance)

npe_arg(p, dense_float, dense_double)
npe_arg(v, dense_float, dense_double)
npe_arg(f, dense_int, dense_long, dense_longlong)
npe_default_arg(return_normals, bool, false)

npe_begin_code()
    assert_cols_equals(p, 3, "p");
    assert_nonzero_rows(p, "p");
    assert_valid_3d_tri_mesh(v, f, "v", "f");

    Eigen::MatrixXd V = v.template cast<double>();
    Eigen::MatrixXd P = p.template cast<double>();
    Eigen::MatrixXi F = f.template cast<int>();

    EigenDenseLike<npe_Matrix_p> S;
    EigenDenseLike<npe_Matrix_f> I;
    EigenDenseLike<npe_Matrix_v> C;
    EigenDenseLike<npe_Matrix_v> N;

    if (return_normals) {
        igl::signed_distance(P, V, F, igl::SIGNED_DISTANCE_TYPE_PSEUDONORMAL, S, I, C, N);
        return pybind11::make_tuple(npe::move(S), npe::move(I), npe::move(C), npe::move(N));
    } else {
        igl::signed_distance(P, V, F, igl::SIGNED_DISTANCE_TYPE_DEFAULT, S, I, C, N);
        return pybind11::make_tuple(npe::move(S), npe::move(I), npe::move(C));
    }

npe_end_code()



