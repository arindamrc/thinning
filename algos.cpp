#include "algos.h"

void ParallelIterativeThinning::compute(const cv::Mat img, cv::Mat result, const ParallelIterativeThinning::ThinningAlgorithm algorithm, bool parallelized)
{
    if (img.type() != CV_8UC1) {
        qCritical() << "Thinning algorithm expects input image to be of type uchar (CV_8UC1)";
        return;
    }
    if (result.size() != img.size() || result.type() != CV_8UC1) {
        result = cv::Mat(img.rows, img.cols, CV_8UC1);
    }
    if (M_.size() != img.size() || M_.type() != CV_8UC1) {
        M_ = cv::Mat(img.rows, img.cols, CV_8UC1);
    }
    if (P_.size() != img.size() || P_.type() != CV_8UC1) {
        P_ = cv::Mat(img.rows, img.cols, CV_8UC1);
    }
    if (D_.size() != img.size() || D_.type() != CV_8UC1) {
        D_ = cv::Mat(img.rows, img.cols, CV_8UC1);
    }

    P_ = cv::Scalar(0);
    result = (img != 0);
    result /= 255;

    parallelized_ = parallelized;
    if (parallelized_)
    {
        cv::setNumThreads(8);
    }

    do {
        iteration(result, algorithm, 0);
        if (algorithm == ZHANG_SUEN_NWSE || algorithm == GUO_HALL || algorithm == LU_WANG || algorithm == KWK || algorithm == BOUDAOUD_SIDER_TARI)
        {
            iteration(result, algorithm, 1);
        }
        cv::absdiff(result, P_, D_);
        result.copyTo(P_);
    } while (cv::countNonZero(D_) > 0);
}

void ParallelIterativeThinning::iteration(cv::Mat I, const ParallelIterativeThinning::ThinningAlgorithm algorithm, const uchar pass)
{
    M_ = cv::Scalar(0);

    parallelized_ = true;

    if (parallelized_)
    {
        I.forEach<uchar>([&, this](const uchar v, const int* pos) -> void {
            Q_UNUSED(v);

            const int i = pos[0];
            const int j = pos[1];

            Neighborhood n;
            getNeighborhood(I,i,j,&n);
            const uchar m = applyAlgorithm(n, algorithm, pass, i, j);
            M_.at<uchar>(i,j) = m;
        });
    } else {
        for (int i = 1; i < I.rows-1; ++i) {
            for (int j = 1; j < I.cols-1; ++j) {

                Neighborhood n;
                getNeighborhood(I, i, j, &n);
                const uchar m = applyAlgorithm(n, algorithm, pass, i, j);
                M_.at<uchar>(i,j) = m;
            }
        }
    }

    I &= M_;
}

uchar ParallelIterativeThinning::applyAlgorithm(const Neighborhood& n, const ParallelIterativeThinning::ThinningAlgorithm algorithm, const uchar pass, const int i, const int j) {
    switch (algorithm) {
    case ZHANG_SUEN_NWSE:
    {
        int A = (n.p2 == 0 && n.p3 == 1) + (n.p3 == 0 && n.p4 == 1) +
                (n.p4 == 0 && n.p5 == 1) + (n.p5 == 0 && n.p6 == 1) +
                (n.p6 == 0 && n.p7 == 1) + (n.p7 == 0 && n.p8 == 1) +
                (n.p8 == 0 && n.p9 == 1) + (n.p9 == 0 && n.p2 == 1);
        int B  = n.p2 + n.p3 + n.p4 + n.p5 + n.p6 + n.p7 + n.p8 + n.p9;
        int m1 = pass == 0 ? (n.p2 * n.p4 * n.p6) : (n.p2 * n.p4 * n.p8);
        int m2 = pass == 0 ? (n.p4 * n.p6 * n.p8) : (n.p2 * n.p6 * n.p8);
        if (n.p1 == 1 && A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0) {
            return 0;
        }
        else {
            return 1;
        }
    }
    break;
    case ZHANG_SUEN_NESW:
    {
        int A = (n.p2 == 0 && n.p3 == 1) + (n.p3 == 0 && n.p4 == 1) +
                (n.p4 == 0 && n.p5 == 1) + (n.p5 == 0 && n.p6 == 1) +
                (n.p6 == 0 && n.p7 == 1) + (n.p7 == 0 && n.p8 == 1) +
                (n.p8 == 0 && n.p9 == 1) + (n.p9 == 0 && n.p2 == 1);
        int B  = n.p2 + n.p3 + n.p4 + n.p5 + n.p6 + n.p7 + n.p8 + n.p9;
        int m1 = pass == 0 ? (n.p2 * n.p6 * n.p8) : (n.p2 * n.p4 * n.p8);
        int m2 = pass == 0 ? (n.p4 * n.p6 * n.p8) : (n.p2 * n.p4 * n.p6);
        if (n.p1 == 1 && A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0) {
            return 0;
        }
        else {
            return 1;
        }
    }
    break;
    case GUO_HALL:
    {
        int C  = ((!n.p2) & (n.p3 | n.p4)) + ((!n.p4) & (n.p5 | n.p6)) +
                ((!n.p6) & (n.p7 | n.p8)) + ((!n.p8) & (n.p9 | n.p2));
        int N1 = (n.p9 | n.p2) + (n.p3 | n.p4) + (n.p5 | n.p6) + (n.p7 | n.p8);
        int N2 = (n.p2 | n.p3) + (n.p4 | n.p5) + (n.p6 | n.p7) + (n.p8 | n.p9);
        int N  = N1 < N2 ? N1 : N2;
        int m  = pass == 0 ? ((n.p6 | n.p7 | (!n.p9)) & n.p8) : ((n.p2 | n.p3 | (!n.p5)) & n.p4);

        if (n.p1 == 1 && (C == 1) && ((N >= 2) && ((N <= 3)) & (m == 0))){
            return 0;
        }
        else {
            return 1;
        }
    }
    break;
    case LU_WANG:
    {
        int A = (n.p2 == 0 && n.p3 == 1) + (n.p3 == 0 && n.p4 == 1) +
                (n.p4 == 0 && n.p5 == 1) + (n.p5 == 0 && n.p6 == 1) +
                (n.p6 == 0 && n.p7 == 1) + (n.p7 == 0 && n.p8 == 1) +
                (n.p8 == 0 && n.p9 == 1) + (n.p9 == 0 && n.p2 == 1);
        int B  = n.p2 + n.p3 + n.p4 + n.p5 + n.p6 + n.p7 + n.p8 + n.p9;
        int m1 = pass == 0 ? (n.p2 * n.p4 * n.p6) : (n.p2 * n.p4 * n.p8);
        int m2 = pass == 0 ? (n.p4 * n.p6 * n.p8) : (n.p2 * n.p6 * n.p8);
        if (n.p1 == 1 && A == 1 && (B >= 3 && B <= 6) && m1 == 0 && m2 == 0) {
            return 0;
        }
        else {
            return 1;
        }
    }
    break;
    case ZHANG_WANG:
    {
        int A = (n.p2 == 0 && n.p3 == 1) + (n.p3 == 0 && n.p4 == 1) +
                (n.p4 == 0 && n.p5 == 1) + (n.p5 == 0 && n.p6 == 1) +
                (n.p6 == 0 && n.p7 == 1) + (n.p7 == 0 && n.p8 == 1) +
                (n.p8 == 0 && n.p9 == 1) + (n.p9 == 0 && n.p2 == 1);
        int B  = n.p2 + n.p3 + n.p4 + n.p5 + n.p6 + n.p7 + n.p8 + n.p9;
        int m1 = (n.p2 * n.p4 * n.p8);
        int m2 = (n.p2 * n.p4 * n.p6);
        if (n.p1 == 1 && A == 1 && (B >= 2 && B <= 6) && (m1 == 0 || n.p11 == 1) && (m2 == 0 || n.p15 == 1)) {
            return 0;
        }
        else {
            return 1;
        }
    }
    break;
    case KWK:
    {
        int A = (n.p2 == 0 && n.p3 == 1) + (n.p3 == 0 && n.p4 == 1) +
                (n.p4 == 0 && n.p5 == 1) + (n.p5 == 0 && n.p6 == 1) +
                (n.p6 == 0 && n.p7 == 1) + (n.p7 == 0 && n.p8 == 1) +
                (n.p8 == 0 && n.p9 == 1) + (n.p9 == 0 && n.p2 == 1);
        int B  = n.p2 + n.p3 + n.p4 + n.p5 + n.p6 + n.p7 + n.p8 + n.p9;
        int m1 = pass == 0 ? (n.p2 * n.p4 * n.p6) : (n.p2 * n.p4 * n.p8);
        int m2 = pass == 0 ? (n.p4 * n.p6 * n.p8) : (n.p2 * n.p6 * n.p8);
        int b = pass == 0 ? (B >= 2 && B <= 6) : (B >= 3 && B <= 6);
        if (n.p1 == 1 && A == 1 && b && m1 == 0 && m2 == 0) {
            return 0;
        }
        else {
            return 1;
        }
    }
    break;
    case HILDITCH:
    {
        int A = (n.p2 == 0 && n.p3 == 1) + (n.p3 == 0 && n.p4 == 1) +
                (n.p4 == 0 && n.p5 == 1) + (n.p5 == 0 && n.p6 == 1) +
                (n.p6 == 0 && n.p7 == 1) + (n.p7 == 0 && n.p8 == 1) +
                (n.p8 == 0 && n.p9 == 1) + (n.p9 == 0 && n.p2 == 1);
        int B  = n.p2 + n.p3 + n.p4 + n.p5 + n.p6 + n.p7 + n.p8 + n.p9;
        int m1 = pass == 0 ? (n.p2 * n.p4 * n.p6) : (n.p2 * n.p4 * n.p8);
        int m2 = pass == 0 ? (n.p4 * n.p6 * n.p8) : (n.p2 * n.p6 * n.p8);
        if (n.p1 == 1 && A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0) {
            return 0;
        }
        else {
            return 1;
        }
    }
    break;
    case APARAJEYA_SANYAL:
    {
        int C1 = n.p1 == 1;
        int C2a = (!n.p4) & n.p8 & ((n.p9 & n.p2 & (!n.p5) & (!n.p6)) | ((!n.p2) & (!n.p3) & n.p6 & n.p7));
        int C2b = n.p6 & n.p7 & n.p8 & ((n.p9 & n.p2 & (!n.p4)) | ((!n.p2) & n.p4 & n.p5));
        int C2c = (!n.p2) & (!n.p3) & (!n.p4) & ((((!n.p5) & (!n.p6) & n.p8) & (n.p9 | n.p7)) | (((!n.p9) & n.p6 & (!n.p8)) & (n.p7 | n.p5)));
        int C2d = n.p4 & (!n.p8) & (((!n.p9) & (!n.p2) & n.p6) | (n.p2 & (!n.p6) & (!n.p7)));
        int C2e = n.p2 & n.p3 & n.p4 & ((n.p5 & n.p6 & (!n.p8)) | (n.p9 & (!n.p6) & n.p8));
        int C2f = (!n.p6) & (!n.p7) & (!n.p8) & (((((!n.p9) & (!n.p2) & n.p4)) & (n.p5 | n.p3)) | ((n.p2 & (!n.p4) & (!n.p5)) & (n.p3 | n.p9)));
        int C3a = n.p9 & n.p2 & n.p6 & n.p7 & n.p8 & (!n.p4) & (!n.p18);
        int C3b = n.p9 & n.p2 & n.p3 & n.p4 & (!n.p6) & n.p8 & (!n.p11);
        int C3c = n.p9 & n.p2 & (!n.p3) & (!n.p4) & (!n.p5) & (!n.p6) & (!n.p7) & n.p8 & (!n.p20) & (!n.p10) & (!n.p11) & (!n.p12) & (!n.p17) & (!n.p18) & (!n.p19);
        int C2 = (C2a || C2b || C2c || C2d || C2e || C2f);
        int C3 = (!(C3a || C3b || C3c));
        if (C1 && C2 && C3) {
            return 0;
        } else {
            return 1;
        }
    }
    break;
    case BOUDAOUD_SIDER_TARI:
    {
        int C  = ((!n.p2) & (n.p3 | n.p4)) + ((!n.p4) & (n.p5 | n.p6)) +
                ((!n.p6) & (n.p7 | n.p8)) + ((!n.p8) & (n.p9 | n.p2));
        int B  = n.p2 + n.p3 + n.p4 + n.p5 + n.p6 + n.p7 + n.p8 + n.p9;
        int m1 = pass == 0 ? (n.p2 * n.p4 * n.p6) : (n.p2 * n.p4 * n.p8);
        int m2 = pass == 0 ? (n.p4 * n.p6 * n.p8) : (n.p2 * n.p6 * n.p8);
        int s = (pass == 0 && ((i + j) % 2 == 0)) || (pass == 1 && ((i + j) % 2 != 0));
        if (n.p1 == 1 && C == 1 && (B >= 2 && B <= 7) && m1 == 0 && m2 == 0 && s) {
            return 0;
        }
        else {
            return 1;
        }
    }
    break;
    default:
        break;
    }

    return 1;
}
