#ifndef ALGOS_H
#define ALGOS_H

#include <QDebug>
#include <type_traits>
#include <set>
#include <opencv2/core.hpp>

class ParallelIterativeThinning
{
public:

    enum ThinningAlgorithm {
        ZHANG_SUEN_NWSE,
        ZHANG_SUEN_NESW,
        GUO_HALL,
        LU_WANG,
        ZHANG_WANG,
        KWK,
        HILDITCH,
        APARAJEYA_SANYAL,
        BOUDAOUD_SIDER_TARI
    };

    ParallelIterativeThinning() {}

    /**
     * @brief compute
     * @param img
     * @param result
     * @param algorithm
     */
    void compute(const cv::Mat img, cv::Mat output, const ThinningAlgorithm algorithm, bool parallelized = false);

private:

    struct Neighborhood
    {
        uchar p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20;
    };

    // Whether the algorithm should be parallelized.
    bool parallelized_;

    // Marker matrix; serves as a buffer for the intermediate steps.
    cv::Mat M_, P_, D_;

    /**
     * @brief iteration
     * @param I
     * @param algorithm
     * @param pass
     */
    void iteration(cv::Mat output, const ThinningAlgorithm algorithm);

    /**
     * @brief _compute
     * @param img
     * @param result
     * @param algorithm
     */
    void subIteration(cv::Mat I, const ThinningAlgorithm algorithm, const uchar pass);

    /**
     * @brief getNeighborhood
     * Get the neighborhood centered on the pixel at (i,j).
     */
    inline void getNeighborhood(const cv::Mat I, const int i, const int j, Neighborhood* n) {
        n->p1 = I.at<uchar>(i, j) != 0;
        n->p2 = I.at<uchar>(i-1, j) != 0;
        n->p3 = I.at<uchar>(i-1, j+1) != 0;
        n->p4 = I.at<uchar>(i, j+1) != 0;
        n->p5 = I.at<uchar>(i+1, j+1) != 0;
        n->p6 = I.at<uchar>(i+1, j) != 0;
        n->p7 = I.at<uchar>(i+1, j-1) != 0;
        n->p8 = I.at<uchar>(i, j-1) != 0;
        n->p9 = I.at<uchar>(i-1, j-1) != 0;
        n->p10 = I.at<uchar>(i-2,j-1) != 0;
        n->p11 = I.at<uchar>(i-2,j) != 0;
        n->p12 = I.at<uchar>(i-2,j+1) != 0;
        n->p13 = I.at<uchar>(i-2,j+2) != 0;
        n->p14 = I.at<uchar>(i-1,j+2) != 0;
        n->p15 = I.at<uchar>(i,j+2) != 0;
        n->p16 = I.at<uchar>(i+1,j+2) != 0;
        n->p17 = I.at<uchar>(i+1,j-2) != 0;
        n->p18 = I.at<uchar>(i,j-2) != 0;
        n->p19 = I.at<uchar>(i-1,j-2) != 0;
        n->p20 = I.at<uchar>(i-2,j-2) != 0;
    }

    /**
     * @brief applyAlgorithm
     * @return The result of the algorithm application.
     */
    uchar applyAlgorithm(const Neighborhood &n, const ThinningAlgorithm algorithm, const uchar pass, const int i, const int j);

};

#endif // ALGOS_H
