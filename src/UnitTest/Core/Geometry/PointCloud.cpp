// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018 www.open3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include "UnitTest.h"
#include "Core/Geometry/PointCloud.h"
#include "Core/Geometry/Image.h"
#include <Core/Geometry/RGBDImage.h>
#include "Core/Camera/PinholeCameraIntrinsic.h"

#include <algorithm>
using namespace std;

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, Constructor)
{
    open3d::PointCloud pc;

    // inherited from Geometry2D
    EXPECT_EQ(open3d::Geometry::GeometryType::PointCloud, pc.GetGeometryType());
    EXPECT_EQ(3, pc.Dimension());

    // public member variables
    EXPECT_EQ(0, pc.points_.size());
    EXPECT_EQ(0, pc.normals_.size());
    EXPECT_EQ(0, pc.colors_.size());

    // public members
    EXPECT_TRUE(pc.IsEmpty());

    unit_test::ExpectEQ(0.0, 0.0, 0.0, pc.GetMinBound());
    unit_test::ExpectEQ(0.0, 0.0, 0.0, pc.GetMaxBound());

    EXPECT_FALSE(pc.HasPoints());
    EXPECT_FALSE(pc.HasNormals());
    EXPECT_FALSE(pc.HasColors());
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, DISABLED_MemberData)
{
    unit_test::NotImplemented();
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, Clear)
{
    int size = 100;

    Eigen::Vector3d vmin(0.0, 0.0, 0.0);
    Eigen::Vector3d vmax(1000.0, 1000.0, 1000.0);

    open3d::PointCloud pc;

    pc.points_.resize(size);
    pc.normals_.resize(size);
    pc.colors_.resize(size);

    unit_test::Rand(pc.points_, vmin, vmax, 0);
    unit_test::Rand(pc.normals_, vmin, vmax, 0);
    unit_test::Rand(pc.colors_, vmin, vmax, 0);

    unit_test::ExpectEQ( 19.607843, 0.0, 0.0, pc.GetMinBound());
    unit_test::ExpectEQ(996.078431, 996.078431, 996.078431, pc.GetMaxBound());

    EXPECT_FALSE(pc.IsEmpty());
    EXPECT_TRUE(pc.HasPoints());
    EXPECT_TRUE(pc.HasNormals());
    EXPECT_TRUE(pc.HasColors());

    pc.Clear();

    // public members
    EXPECT_TRUE(pc.IsEmpty());

    unit_test::ExpectEQ(0.0, 0.0, 0.0, pc.GetMinBound());
    unit_test::ExpectEQ(0.0, 0.0, 0.0, pc.GetMaxBound());

    EXPECT_FALSE(pc.HasPoints());
    EXPECT_FALSE(pc.HasNormals());
    EXPECT_FALSE(pc.HasColors());
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, IsEmpty)
{
    int size = 100;

    Eigen::Vector3d vmin(0.0, 0.0, 0.0);
    Eigen::Vector3d vmax(1000.0, 1000.0, 1000.0);

    open3d::PointCloud pc;

    EXPECT_TRUE(pc.IsEmpty());

    pc.points_.resize(size);

    unit_test::Rand(pc.points_, vmin, vmax, 0);

    EXPECT_FALSE(pc.IsEmpty());
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, GetMinBound)
{
    int size = 100;

    Eigen::Vector3d vmin(0.0, 0.0, 0.0);
    Eigen::Vector3d vmax(1000.0, 1000.0, 1000.0);

    open3d::PointCloud pc;

    pc.points_.resize(size);

    unit_test::Rand(pc.points_, vmin, vmax, 0);

    Eigen::Vector3d minBound = pc.GetMinBound();

    unit_test::ExpectEQ( 19.607843, 0.0, 0.0, pc.GetMinBound());
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, GetMaxBound)
{
    int size = 100;

    Eigen::Vector3d vmin(0.0, 0.0, 0.0);
    Eigen::Vector3d vmax(1000.0, 1000.0, 1000.0);

    open3d::PointCloud pc;

    pc.points_.resize(size);

    unit_test::Rand(pc.points_, vmin, vmax, 0);

    Eigen::Vector3d maxBound = pc.GetMaxBound();

    unit_test::ExpectEQ(996.078431, 996.078431, 996.078431, pc.GetMaxBound());
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, Transform)
{
    vector<Eigen::Vector3d> ref_points =
    {
        {  396.870588, 1201.976471,  880.472941 },
        {  320.792157, 1081.976471,  829.139608 },
        {  269.027451,  818.447059,  406.786667 },
        {  338.831373, 1001.192157,  614.237647 },
        {  423.537255, 1153.349020,  483.727843 },
        {  432.949020, 1338.447059,  964.512157 },
        {  140.007843,  444.721569,  189.296471 },
        {  292.164706,  763.152941,  317.178824 },
        {  134.517647,  407.858824,  192.002353 },
        {  274.909804,  802.368627,  218.747451 }
    };

    vector<Eigen::Vector3d> ref_normals =
    {
        {  396.470588, 1201.176471,  880.352941 },
        {  320.392157, 1081.176471,  829.019608 },
        {  268.627451,  817.647059,  406.666667 },
        {  338.431373, 1000.392157,  614.117647 },
        {  423.137255, 1152.549020,  483.607843 },
        {  432.549020, 1337.647059,  964.392157 },
        {  139.607843,  443.921569,  189.176471 },
        {  291.764706,  762.352941,  317.058824 },
        {  134.117647,  407.058824,  191.882353 },
        {  274.509804,  801.568627,  218.627451 }
    };

    int size = 10;
    open3d::PointCloud pc;

    Eigen::Vector3d vmin(0.0, 0.0, 0.0);
    Eigen::Vector3d vmax(1000.0, 1000.0, 1000.0);

    pc.points_.resize(size);
    unit_test::Rand(pc.points_, vmin, vmax, 0);

    pc.normals_.resize(size);
    unit_test::Rand(pc.normals_, vmin, vmax, 0);

    Eigen::Matrix4d transformation;
    transformation << 0.10, 0.20, 0.30, 0.40,
                      0.50, 0.60, 0.70, 0.80,
                      0.90, 0.10, 0.11, 0.12,
                      0.13, 0.14, 0.15, 0.16;

    pc.Transform(transformation);

    EXPECT_EQ(ref_points.size(), pc.points_.size());
    EXPECT_EQ(ref_normals.size(), pc.normals_.size());
    for (size_t i = 0; i < pc.points_.size(); i++)
    {
        unit_test::ExpectEQ(ref_points[i], pc.points_[i]);
        unit_test::ExpectEQ(ref_normals[i], pc.normals_[i]);
    }
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, HasPoints)
{
    int size = 100;

    open3d::PointCloud pc;

    EXPECT_FALSE(pc.HasPoints());

    pc.points_.resize(size);

    EXPECT_TRUE(pc.HasPoints());
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, HasNormals)
{
    int size = 100;

    open3d::PointCloud pc;

    EXPECT_FALSE(pc.HasNormals());

    pc.points_.resize(size);
    pc.normals_.resize(size);

    EXPECT_TRUE(pc.HasNormals());
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, HasColors)
{
    int size = 100;

    open3d::PointCloud pc;

    EXPECT_FALSE(pc.HasColors());

    pc.points_.resize(size);
    pc.colors_.resize(size);

    EXPECT_TRUE(pc.HasColors());
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, NormalizeNormals)
{
    vector<Eigen::Vector3d> ref =
    {
        {    0.692861,    0.323767,    0.644296 },
        {    0.650010,    0.742869,    0.160101 },
        {    0.379563,    0.870761,    0.312581 },
        {    0.575046,    0.493479,    0.652534 },
        {    0.320665,    0.448241,    0.834418 },
        {    0.691127,    0.480526,    0.539850 },
        {    0.227557,    0.973437,    0.025284 },
        {    0.281666,    0.156994,    0.946582 },
        {    0.341869,    0.894118,    0.289273 },
        {    0.103335,    0.972118,    0.210498 },
        {    0.441745,    0.723783,    0.530094 },
        {    0.336903,    0.727710,    0.597441 },
        {    0.434917,    0.862876,    0.257471 },
        {    0.636619,    0.435239,    0.636619 },
        {    0.393717,    0.876213,    0.277918 },
        {    0.275051,    0.633543,    0.723167 },
        {    0.061340,    0.873191,    0.483503 },
        {    0.118504,    0.276510,    0.953677 },
        {    0.930383,    0.360677,    0.065578 },
        {    0.042660,    0.989719,    0.136513 }
    };

    int size = 20;

    Eigen::Vector3d vmin(0.0, 0.0, 0.0);
    Eigen::Vector3d vmax(1000.0, 1000.0, 1000.0);

    open3d::PointCloud pc;

    pc.normals_.resize(size);

    unit_test::Rand(pc.normals_, vmin, vmax, 0);

    pc.NormalizeNormals();

    EXPECT_EQ(ref.size(), pc.normals_.size());
    for (size_t i = 0; i < pc.normals_.size(); i++)
        unit_test::ExpectEQ(ref[i], pc.normals_[i]);
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, PaintUniformColor)
{
    int size = 100;

    Eigen::Vector3d vmin(0.0, 0.0, 0.0);
    Eigen::Vector3d vmax(1000.0, 1000.0, 1000.0);

    open3d::PointCloud pc;

    EXPECT_TRUE(pc.IsEmpty());

    pc.points_.resize(size);

    unit_test::Rand(pc.points_, vmin, vmax, 0);

    EXPECT_FALSE(pc.HasColors());

    pc.PaintUniformColor(Eigen::Vector3d(233, 171, 53));

    EXPECT_TRUE(pc.HasColors());

    for (size_t i = 0; i < pc.colors_.size(); i++)
        unit_test::ExpectEQ(233, 171, 53, pc.colors_[i]);
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, OperatorAppend)
{
    int size = 100;

    open3d::PointCloud pc0;
    open3d::PointCloud pc1;

    pc0.points_.resize(size);
    pc0.normals_.resize(size);
    pc0.colors_.resize(size);

    pc1.points_.resize(size);
    pc1.normals_.resize(size);
    pc1.colors_.resize(size);

    unit_test::Rand(pc0.points_, Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(1000.0, 1000.0, 1000.0), 0);
    unit_test::Rand(pc0.normals_, Eigen::Vector3d(-1.0, -1.0, -1.0), Eigen::Vector3d(1.0, 1.0, 1.0), 0);
    unit_test::Rand(pc0.colors_, Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(1.0, 1.0, 1.0), 0);

    unit_test::Rand(pc1.points_, Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(1000.0, 1000.0, 1000.0), 0);
    unit_test::Rand(pc1.normals_, Eigen::Vector3d(-1.0, -1.0, -1.0), Eigen::Vector3d(1.0, 1.0, 1.0), 0);
    unit_test::Rand(pc1.colors_, Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(1.0, 1.0, 1.0), 1);

    vector<Eigen::Vector3d> p;
    p.insert(p.end(), pc0.points_.begin(), pc0.points_.end());
    p.insert(p.end(), pc1.points_.begin(), pc1.points_.end());

    vector<Eigen::Vector3d> n;
    n.insert(n.end(), pc0.normals_.begin(), pc0.normals_.end());
    n.insert(n.end(), pc1.normals_.begin(), pc1.normals_.end());

    vector<Eigen::Vector3d> c;
    c.insert(c.end(), pc0.colors_.begin(), pc0.colors_.end());
    c.insert(c.end(), pc1.colors_.begin(), pc1.colors_.end());

    open3d::PointCloud pc(pc0);
    pc += pc1;

    EXPECT_EQ(2 * size, pc.points_.size());
    for (size_t i = 0; i < size; i++)
    {
        unit_test::ExpectEQ(pc0.points_[i], pc.points_[   0 + i]);
        unit_test::ExpectEQ(pc1.points_[i], pc.points_[size + i]);
    }

    EXPECT_EQ(2 * size, pc.normals_.size());
    for (size_t i = 0; i < size; i++)
    {
        unit_test::ExpectEQ(pc0.normals_[i], pc.normals_[   0 + i]);
        unit_test::ExpectEQ(pc1.normals_[i], pc.normals_[size + i]);
    }

    EXPECT_EQ(2 * size, pc.colors_.size());
    for (size_t i = 0; i < size; i++)
    {
        unit_test::ExpectEQ(pc0.colors_[i], pc.colors_[   0 + i]);
        unit_test::ExpectEQ(pc1.colors_[i], pc.colors_[size + i]);
    }
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, OperatorADD)
{
    int size = 100;

    open3d::PointCloud pc0;
    open3d::PointCloud pc1;

    pc0.points_.resize(size);
    pc0.normals_.resize(size);
    pc0.colors_.resize(size);

    pc1.points_.resize(size);
    pc1.normals_.resize(size);
    pc1.colors_.resize(size);

    unit_test::Rand(pc0.points_, Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(1000.0, 1000.0, 1000.0), 0);
    unit_test::Rand(pc0.normals_, Eigen::Vector3d(-1.0, -1.0, -1.0), Eigen::Vector3d(1.0, 1.0, 1.0), 0);
    unit_test::Rand(pc0.colors_, Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(1.0, 1.0, 1.0), 0);

    unit_test::Rand(pc1.points_, Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(1000.0, 1000.0, 1000.0), 0);
    unit_test::Rand(pc1.normals_, Eigen::Vector3d(-1.0, -1.0, -1.0), Eigen::Vector3d(1.0, 1.0, 1.0), 0);
    unit_test::Rand(pc1.colors_, Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(1.0, 1.0, 1.0), 1);

    vector<Eigen::Vector3d> p;
    p.insert(p.end(), pc0.points_.begin(), pc0.points_.end());
    p.insert(p.end(), pc1.points_.begin(), pc1.points_.end());

    vector<Eigen::Vector3d> n;
    n.insert(n.end(), pc0.normals_.begin(), pc0.normals_.end());
    n.insert(n.end(), pc1.normals_.begin(), pc1.normals_.end());

    vector<Eigen::Vector3d> c;
    c.insert(c.end(), pc0.colors_.begin(), pc0.colors_.end());
    c.insert(c.end(), pc1.colors_.begin(), pc1.colors_.end());

    open3d::PointCloud pc = pc0 + pc1;

    EXPECT_EQ(2 * size, pc.points_.size());
    for (size_t i = 0; i < size; i++)
    {
        unit_test::ExpectEQ(pc0.points_[i], pc.points_[   0 + i]);
        unit_test::ExpectEQ(pc1.points_[i], pc.points_[size + i]);
    }

    EXPECT_EQ(2 * size, pc.normals_.size());
    for (size_t i = 0; i < size; i++)
    {
        unit_test::ExpectEQ(pc0.normals_[i], pc.normals_[   0 + i]);
        unit_test::ExpectEQ(pc1.normals_[i], pc.normals_[size + i]);
    }

    EXPECT_EQ(2 * size, pc.colors_.size());
    for (size_t i = 0; i < size; i++)
    {
        unit_test::ExpectEQ(pc0.colors_[i], pc.colors_[   0 + i]);
        unit_test::ExpectEQ(pc1.colors_[i], pc.colors_[size + i]);
    }
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, DISABLED_CreatePointCloudFromFile)
{
    unit_test::NotImplemented();
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, SelectDownSample)
{
    vector<Eigen::Vector3d> ref =
    {
        {  694.117647,  670.588235,  635.294118 },
        {  415.686275,  168.627451,  905.882353 },
        {  431.372549,    0.000000,  341.176471 },
        {  596.078431,  831.372549,  231.372549 },
        {  109.803922,  360.784314,  576.470588 },
        {   19.607843,  454.901961,   62.745098 },
        {  909.803922,  482.352941,  215.686275 },
        {  470.588235,  592.156863,  941.176471 },
        {  164.705882,  439.215686,  878.431373 },
        {  203.921569,  886.274510,  121.568627 },
        {  949.019608,   50.980392,  517.647059 },
        {  529.411765,  756.862745,  301.960784 },
        {  615.686275,  278.431373,  784.313725 },
        {  639.215686,  756.862745,   90.196078 },
        {  823.529412,  329.411765,  184.313725 },
        {  592.156863,  662.745098,  286.274510 },
        {  992.156863,  576.470588,  874.509804 },
        {  227.450980,  698.039216,  313.725490 },
        {  400.000000,  890.196078,  282.352941 },
        {  356.862745,  549.019608,  576.470588 },
        {  796.078431,  909.803922,  196.078431 },
        {  666.666667,  529.411765,   39.215686 },
        {  768.627451,  525.490196,  768.627451 },
        {  674.509804,  482.352941,  478.431373 },
        {  349.019608,  803.921569,  917.647059 }
    };

    int size = 100;
    open3d::PointCloud pc;

    Eigen::Vector3d vmin(0.0, 0.0, 0.0);
    Eigen::Vector3d vmax(1000.0, 1000.0, 1000.0);

    pc.points_.resize(size);
    unit_test::Rand(pc.points_, vmin, vmax, 0);

    vector<size_t> indices(size / 4);
    unit_test::Rand(indices, 0, size, 0);

    // remove duplicates
    std::vector<size_t>::iterator it;
    it = unique(indices.begin(), indices.end());
    indices.resize(distance(indices.begin(), it));

    auto output_pc = open3d::SelectDownSample(pc, indices);

    EXPECT_EQ(ref.size(), output_pc->points_.size());
    for (size_t i = 0; i < indices.size(); i++)
        unit_test::ExpectEQ(ref[i], output_pc->points_[i]);
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, VoxelDownSample)
{
    vector<Eigen::Vector3d> ref_points =
    {
        {   19.607843,  454.901961,   62.745098 },
        {   66.666667,  949.019608,  525.490196 },
        {   82.352941,  192.156863,  662.745098 },
        {  105.882353,  996.078431,  215.686275 },
        {  141.176471,  603.921569,   15.686275 },
        {  152.941176,  400.000000,  129.411765 },
        {  239.215686,  133.333333,  803.921569 },
        {  294.117647,  635.294118,  521.568627 },
        {  333.333333,  764.705882,  274.509804 },
        {  349.019608,  803.921569,  917.647059 },
        {  364.705882,  509.803922,  949.019608 },
        {  400.000000,  890.196078,  282.352941 },
        {  490.196078,  972.549020,  290.196078 },
        {  509.803922,  835.294118,  611.764706 },
        {  552.941176,  474.509804,  627.450980 },
        {  768.627451,  525.490196,  768.627451 },
        {  796.078431,  909.803922,  196.078431 },
        {  839.215686,  392.156863,  780.392157 },
        {  890.196078,  345.098039,   62.745098 },
        {  913.725490,  635.294118,  713.725490 }
    };

    vector<Eigen::Vector3d> ref_normals =
    {
        {    0.042660,    0.989719,    0.136513 },
        {    0.061340,    0.873191,    0.483503 },
        {    0.103335,    0.972118,    0.210498 },
        {    0.118504,    0.276510,    0.953677 },
        {    0.227557,    0.973437,    0.025284 },
        {    0.275051,    0.633543,    0.723167 },
        {    0.281666,    0.156994,    0.946582 },
        {    0.320665,    0.448241,    0.834418 },
        {    0.336903,    0.727710,    0.597441 },
        {    0.341869,    0.894118,    0.289273 },
        {    0.379563,    0.870761,    0.312581 },
        {    0.393717,    0.876213,    0.277918 },
        {    0.434917,    0.862876,    0.257471 },
        {    0.441745,    0.723783,    0.530094 },
        {    0.575046,    0.493479,    0.652534 },
        {    0.636619,    0.435239,    0.636619 },
        {    0.650010,    0.742869,    0.160101 },
        {    0.691127,    0.480526,    0.539850 },
        {    0.692861,    0.323767,    0.644296 },
        {    0.930383,    0.360677,    0.065578 }
    };

    vector<Eigen::Vector3d> ref_colors =
    {
        {    5.000000,  116.000000,   16.000000 },
        {   17.000000,  242.000000,  134.000000 },
        {   21.000000,   49.000000,  169.000000 },
        {   27.000000,  254.000000,   55.000000 },
        {   36.000000,  154.000000,    4.000000 },
        {   39.000000,  102.000000,   33.000000 },
        {   61.000000,   34.000000,  205.000000 },
        {   75.000000,  162.000000,  133.000000 },
        {   85.000000,  195.000000,   70.000000 },
        {   89.000000,  205.000000,  234.000000 },
        {   93.000000,  130.000000,  242.000000 },
        {  102.000000,  227.000000,   72.000000 },
        {  125.000000,  248.000000,   74.000000 },
        {  130.000000,  213.000000,  156.000000 },
        {  141.000000,  121.000000,  160.000000 },
        {  196.000000,  134.000000,  196.000000 },
        {  203.000000,  232.000000,   50.000000 },
        {  214.000000,  100.000000,  199.000000 },
        {  227.000000,   88.000000,   16.000000 },
        {  233.000000,  162.000000,  182.000000 }
    };

    int size = 20;
    open3d::PointCloud pc;

    pc.points_.resize(size);
    pc.normals_.resize(size);
    pc.colors_.resize(size);

    unit_test::Rand(pc.points_, Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(1000.0, 1000.0, 1000.0), 0);
    unit_test::Rand(pc.normals_, Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(10.0, 10.0, 10.0), 0);
    unit_test::Rand(pc.colors_, Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(255.0, 255.0, 255.0), 0);

    double voxel_size = 0.5;
    auto output_pc = open3d::VoxelDownSample(pc, voxel_size);

    // sometimes the order of these Eigen::Vector3d values can be mixed-up
    // sort these vectors in order to match the expected order.
    unit_test::Sort::Do(output_pc->points_);
    unit_test::Sort::Do(output_pc->normals_);
    unit_test::Sort::Do(output_pc->colors_);

    EXPECT_EQ(ref_points.size(), output_pc->points_.size());
    EXPECT_EQ(ref_normals.size(), output_pc->normals_.size());
    EXPECT_EQ(ref_colors.size(), output_pc->colors_.size());
    for (size_t i = 0; i < output_pc->points_.size(); i++)
    {
        unit_test::ExpectEQ(ref_points[i], output_pc->points_[i]);
        unit_test::ExpectEQ(ref_normals[i], output_pc->normals_[i]);
        unit_test::ExpectEQ(ref_colors[i], output_pc->colors_[i]);
    }
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, UniformDownSample)
{
    vector<Eigen::Vector3d> ref =
    {
        {  839.215686,  392.156863,  780.392157 },
        {  364.705882,  509.803922,  949.019608 },
        {  152.941176,  400.000000,  129.411765 },
        {  490.196078,  972.549020,  290.196078 },
        {   66.666667,  949.019608,  525.490196 },
        {  235.294118,  968.627451,  901.960784 },
        {  435.294118,  929.411765,  929.411765 },
        {  827.450980,  329.411765,  227.450980 },
        {  396.078431,  811.764706,  682.352941 },
        {  615.686275,  278.431373,  784.313725 },
        {  101.960784,  125.490196,  494.117647 },
        {  584.313725,  243.137255,  149.019608 },
        {  172.549020,  239.215686,  796.078431 },
        {   66.666667,  203.921569,  458.823529 },
        {  996.078431,   50.980392,  866.666667 },
        {  356.862745,  549.019608,  576.470588 },
        {  745.098039,  627.450980,   35.294118 },
        {  666.666667,  494.117647,  160.784314 },
        {  325.490196,  231.372549,   70.588235 },
        {  470.588235,  592.156863,  941.176471 },
        {  674.509804,  482.352941,  478.431373 },
        {  345.098039,  184.313725,  607.843137 },
        {  529.411765,   86.274510,  258.823529 },
        {  772.549020,  286.274510,  329.411765 },
        {  764.705882,  698.039216,  117.647059 }
    };

    int size = 100;
    open3d::PointCloud pc;

    Eigen::Vector3d vmin(0.0, 0.0, 0.0);
    Eigen::Vector3d vmax(1000.0, 1000.0, 1000.0);

    pc.points_.resize(size);
    unit_test::Rand(pc.points_, vmin, vmax, 0);

    size_t every_k_points = 4;
    auto output_pc = open3d::UniformDownSample(pc, every_k_points);

    EXPECT_EQ(ref.size(), output_pc->points_.size());
    for (size_t i = 0; i < output_pc->points_.size(); i++)
        unit_test::ExpectEQ(ref[i], output_pc->points_[i]);
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, CropPointCloud)
{
    int size = 100;
    open3d::PointCloud pc;

    Eigen::Vector3d vmin(0.0, 0.0, 0.0);
    Eigen::Vector3d vmax(1000.0, 1000.0, 1000.0);

    pc.points_.resize(size);
    unit_test::Rand(pc.points_, vmin, vmax, 0);

    Eigen::Vector3d minBound(200.0, 200.0, 200.0);
    Eigen::Vector3d maxBound(800.0, 800.0, 800.0);
    auto output_pc = open3d::CropPointCloud(pc, minBound, maxBound);

    for (size_t i = 0; i < output_pc->points_.size(); i++)
    {
        unit_test::ExpectLE(minBound, output_pc->points_[i]);
        unit_test::ExpectGE(maxBound, output_pc->points_[i]);
    }
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, EstimateNormals)
{
    vector<Eigen::Vector3d> ref =
    {
        {    0.282003,    0.866394,    0.412111 },
        {    0.550791,    0.829572,   -0.091869 },
        {    0.076085,   -0.974168,    0.212620 },
        {    0.261265,    0.825182,    0.500814 },
        {    0.035397,    0.428362,    0.902913 },
        {    0.711421,    0.595291,    0.373508 },
        {    0.519141,    0.552592,    0.652024 },
        {    0.490520,    0.573293,   -0.656297 },
        {    0.324029,    0.744177,    0.584128 },
        {    0.120589,   -0.989854,    0.075152 },
        {    0.370700,    0.767066,    0.523632 },
        {    0.874692,   -0.158725,   -0.457952 },
        {    0.238700,    0.937064,   -0.254819 },
        {    0.518237,    0.540189,    0.663043 },
        {    0.238700,    0.937064,   -0.254819 },
        {    0.080943,   -0.502095,   -0.861016 },
        {    0.753661,   -0.527376,   -0.392261 },
        {    0.721099,    0.542859,   -0.430489 },
        {    0.159997,   -0.857801,   -0.488446 },
        {    0.445869,    0.725107,    0.524805 },
        {    0.019474,   -0.592041,   -0.805672 },
        {    0.024464,    0.856206,    0.516056 },
        {    0.478041,    0.869593,   -0.123631 },
        {    0.104534,   -0.784980,   -0.610638 },
        {    0.073901,    0.570353,    0.818069 },
        {    0.178678,    0.974506,    0.135693 },
        {    0.178678,    0.974506,    0.135693 },
        {    0.581675,    0.167795,   -0.795926 },
        {    0.069588,   -0.845043,   -0.530150 },
        {    0.626448,    0.486534,    0.608973 },
        {    0.670665,    0.657002,    0.344321 },
        {    0.588868,    0.011829,    0.808143 },
        {    0.081974,    0.638039,    0.765628 },
        {    0.159997,   -0.857801,   -0.488446 },
        {    0.559499,    0.824271,   -0.086826 },
        {    0.612885,    0.727999,    0.307229 },
        {    0.178678,    0.974506,    0.135693 },
        {    0.268803,    0.796616,    0.541431 },
        {    0.604933,    0.787776,   -0.116044 },
        {    0.111998,    0.869999,   -0.480165 }
    };

    int size = 40;
    open3d::PointCloud pc;

    Eigen::Vector3d vmin(0.0, 0.0, 0.0);
    Eigen::Vector3d vmax(1000.0, 1000.0, 1000.0);

    pc.points_.resize(size);
    unit_test::Rand(pc.points_, vmin, vmax, 0);

    bool result = open3d::EstimateNormals(pc);

    EXPECT_EQ(ref.size(), pc.normals_.size());
    for (size_t i = 0; i < pc.normals_.size(); i++)
        unit_test::ExpectEQ(ref[i], pc.normals_[i]);
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, OrientNormalsToAlignWithDirection)
{
    vector<Eigen::Vector3d> ref =
    {
        {    0.282003,    0.866394,    0.412111 },
        {    0.550791,    0.829572,   -0.091869 },
        {    0.076085,   -0.974168,    0.212620 },
        {    0.261265,    0.825182,    0.500814 },
        {    0.035397,    0.428362,    0.902913 },
        {    0.711421,    0.595291,    0.373508 },
        {    0.519141,    0.552592,    0.652024 },
        {   -0.490520,   -0.573293,    0.656297 },
        {    0.324029,    0.744177,    0.584128 },
        {   -0.120589,    0.989854,   -0.075152 },
        {    0.370700,    0.767066,    0.523632 },
        {   -0.874692,    0.158725,    0.457952 },
        {   -0.238700,   -0.937064,    0.254819 },
        {    0.518237,    0.540189,    0.663043 },
        {   -0.238700,   -0.937064,    0.254819 },
        {   -0.080943,    0.502095,    0.861016 },
        {   -0.753661,    0.527376,    0.392261 },
        {   -0.721099,   -0.542859,    0.430489 },
        {   -0.159997,    0.857801,    0.488446 },
        {    0.445869,    0.725107,    0.524805 },
        {   -0.019474,    0.592041,    0.805672 },
        {    0.024464,    0.856206,    0.516056 },
        {    0.478041,    0.869593,   -0.123631 },
        {   -0.104534,    0.784980,    0.610638 },
        {    0.073901,    0.570353,    0.818069 },
        {    0.178678,    0.974506,    0.135693 },
        {    0.178678,    0.974506,    0.135693 },
        {   -0.581675,   -0.167795,    0.795926 },
        {   -0.069588,    0.845043,    0.530150 },
        {    0.626448,    0.486534,    0.608973 },
        {    0.670665,    0.657002,    0.344321 },
        {    0.588868,    0.011829,    0.808143 },
        {    0.081974,    0.638039,    0.765628 },
        {   -0.159997,    0.857801,    0.488446 },
        {    0.559499,    0.824271,   -0.086826 },
        {    0.612885,    0.727999,    0.307229 },
        {    0.178678,    0.974506,    0.135693 },
        {    0.268803,    0.796616,    0.541431 },
        {    0.604933,    0.787776,   -0.116044 },
        {   -0.111998,   -0.869999,    0.480165 }
    };

    int size = 40;
    open3d::PointCloud pc;

    Eigen::Vector3d vmin(0.0, 0.0, 0.0);
    Eigen::Vector3d vmax(1000.0, 1000.0, 1000.0);

    pc.points_.resize(size);
    unit_test::Rand(pc.points_, vmin, vmax, 0);

    bool result = open3d::EstimateNormals(pc);
    result = open3d::OrientNormalsToAlignWithDirection(pc, Eigen::Vector3d(1.5, 0.5, 3.3));

    EXPECT_EQ(ref.size(), pc.normals_.size());
    for (size_t i = 0; i < pc.normals_.size(); i++)
        unit_test::ExpectEQ(ref[i], pc.normals_[i]);
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, OrientNormalsTowardsCameraLocation)
{
    vector<Eigen::Vector3d> ref =
    {
        {   -0.282003,   -0.866394,   -0.412111 },
        {   -0.550791,   -0.829572,    0.091869 },
        {    0.076085,   -0.974168,    0.212620 },
        {   -0.261265,   -0.825182,   -0.500814 },
        {   -0.035397,   -0.428362,   -0.902913 },
        {   -0.711421,   -0.595291,   -0.373508 },
        {   -0.519141,   -0.552592,   -0.652024 },
        {    0.490520,    0.573293,   -0.656297 },
        {   -0.324029,   -0.744177,   -0.584128 },
        {    0.120589,   -0.989854,    0.075152 },
        {   -0.370700,   -0.767066,   -0.523632 },
        {    0.874692,   -0.158725,   -0.457952 },
        {   -0.238700,   -0.937064,    0.254819 },
        {   -0.518237,   -0.540189,   -0.663043 },
        {   -0.238700,   -0.937064,    0.254819 },
        {    0.080943,   -0.502095,   -0.861016 },
        {    0.753661,   -0.527376,   -0.392261 },
        {    0.721099,    0.542859,   -0.430489 },
        {    0.159997,   -0.857801,   -0.488446 },
        {   -0.445869,   -0.725107,   -0.524805 },
        {    0.019474,   -0.592041,   -0.805672 },
        {   -0.024464,   -0.856206,   -0.516056 },
        {   -0.478041,   -0.869593,    0.123631 },
        {    0.104534,   -0.784980,   -0.610638 },
        {   -0.073901,   -0.570353,   -0.818069 },
        {   -0.178678,   -0.974506,   -0.135693 },
        {   -0.178678,   -0.974506,   -0.135693 },
        {    0.581675,    0.167795,   -0.795926 },
        {    0.069588,   -0.845043,   -0.530150 },
        {   -0.626448,   -0.486534,   -0.608973 },
        {   -0.670665,   -0.657002,   -0.344321 },
        {   -0.588868,   -0.011829,   -0.808143 },
        {   -0.081974,   -0.638039,   -0.765628 },
        {    0.159997,   -0.857801,   -0.488446 },
        {   -0.559499,   -0.824271,    0.086826 },
        {   -0.612885,   -0.727999,   -0.307229 },
        {   -0.178678,   -0.974506,   -0.135693 },
        {   -0.268803,   -0.796616,   -0.541431 },
        {   -0.604933,   -0.787776,    0.116044 },
        {    0.111998,    0.869999,   -0.480165 }
    };

    int size = 40;
    open3d::PointCloud pc;

    Eigen::Vector3d vmin(0.0, 0.0, 0.0);
    Eigen::Vector3d vmax(1000.0, 1000.0, 1000.0);

    pc.points_.resize(size);
    unit_test::Rand(pc.points_, vmin, vmax, 0);

    bool result = open3d::EstimateNormals(pc);
    result = open3d::OrientNormalsTowardsCameraLocation(pc, Eigen::Vector3d(1.5, 0.5, 3.3));

    EXPECT_EQ(ref.size(), pc.normals_.size());
    for (size_t i = 0; i < pc.normals_.size(); i++)
        unit_test::ExpectEQ(ref[i], pc.normals_[i]);
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, ComputePointCloudToPointCloudDistance)
{
    vector<double> ref =
    {
        157.498711,  127.737235,  113.386920,  192.476725,  134.367386,
        119.720294,  104.713960,  228.597516,  131.299365,  174.718976,
        248.300645,  119.976930,   23.200313,   71.130812,  134.995408,
        149.534713,  206.804657,  191.395049,  139.532917,  130.417954,
        183.393615,  219.957705,  179.623255,  125.612685,   29.865777,
        110.013805,  100.287936,  303.180627,   43.315141,  227.214211,
        166.239360,  199.730691,  168.855295,  178.377497,  144.034256,
        261.542463,  122.388130,  239.857705,  116.398605,  177.686443,
         92.966820,   96.138437,   23.529412,  177.902686,   68.149597,
        148.191715,  158.520650,  346.210329,  248.114768,  182.342399
    };

    int size = 100;

    open3d::PointCloud pc0;
    open3d::PointCloud pc1;

    Eigen::Vector3d vmin(0.0, 0.0, 0.0);
    Eigen::Vector3d vmax(1000.0, 1000.0, 1000.0);

    vector<Eigen::Vector3d> points(size);
    unit_test::Rand(points, vmin, vmax, 0);

    for (int i = 0; i < (size / 2); i++)
    {
        pc0.points_.push_back(points[         0 + i]);
        pc1.points_.push_back(points[(size / 2) + i]);
    }

    vector<double> distance = open3d::ComputePointCloudToPointCloudDistance(pc0, pc1);

    EXPECT_EQ(ref.size(), distance.size());
    for (size_t i = 0; i < distance.size(); i++)
        EXPECT_NEAR(ref[i], distance[i], unit_test::THRESHOLD_1E_6);
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, ComputePointCloudMeanAndCovariance)
{
    int size = 40;
    open3d::PointCloud pc;

    Eigen::Vector3d vmin(0.0, 0.0, 0.0);
    Eigen::Vector3d vmax(1000.0, 1000.0, 1000.0);

    pc.points_.resize(size);
    unit_test::Rand(pc.points_, vmin, vmax, 0);

    auto output = open3d::ComputePointCloudMeanAndCovariance(pc);

    Eigen::Vector3d mean = get<0>(output);
    Eigen::Matrix3d covariance = get<1>(output);

    unit_test::ExpectEQ(514.215686, 566.666666, 526.568627, mean);

    Eigen::Matrix3d ref_covariance;
    ref_covariance << 86747.549019, -9480.776624, 1416.234140,
                      -9480.776624, 64536.716647, -12861.399461,
                      1416.234140, -12861.399461, 85923.096885;

    unit_test::ExpectEQ(ref_covariance, covariance);
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, ComputePointCloudMahalanobisDistance)
{
    vector<double> ref =
    {
        1.439881,    1.872615,    1.232338,    0.437462,    1.617472,
        1.556793,    2.019575,    1.984814,    1.845557,    2.340981,
        1.177214,    0.960687,    1.629667,    1.165109,    1.443769,
        1.926848,    2.299014,    2.096105,    2.208119,    2.279263,
        2.473191,    1.520355,    0.999211,    1.627128,    2.151709,
        1.274612,    0.898971,    1.899552,    1.705108,    1.527357,
        1.594832,    1.724115,    1.337456,    1.771533,    2.323719,
        1.352616,    1.234500,    1.233509,    1.568047,    1.804558,
        2.123548,    2.340709,    1.036468,    1.387644,    1.668290,
        1.780892,    1.934451,    2.334212,    1.918557,    1.701092,
        1.544488,    1.918934,    2.054735,    1.286736,    2.717411,
        1.984941,    2.560241,    2.818804,    1.824099,    1.899621,
        0.710997,    1.358864,    0.957766,    1.996037,    1.761726,
        1.921903,    2.229389,    2.221731,    1.281107,    2.112245,
        1.135575,    1.329718,    1.957294,    1.208041,    1.624905,
        1.358104,    1.500636,    1.311195,    2.056985,    1.318757,
        0.579100,    1.370243,    1.871690,    0.875355,    1.431115,
        0.921470,    1.807282,    1.054253,    1.871363,    1.939949,
        1.681727,    0.831352,    1.466078,    2.420450,    1.787704,
        2.197718,    1.620004,    1.087840,    2.319767,    1.569915
    };

    int size = 100;

    open3d::PointCloud pc;

    Eigen::Vector3d vmin(0.0, 0.0, 0.0);
    Eigen::Vector3d vmax(1000.0, 1000.0, 1000.0);

    pc.points_.resize(size);
    unit_test::Rand(pc.points_, vmin, vmax, 0);

    vector<double> distance = open3d::ComputePointCloudMahalanobisDistance(pc);

    EXPECT_EQ(ref.size(), distance.size());
    for (size_t i = 0; i < distance.size(); i++)
        EXPECT_NEAR(ref[i], distance[i], unit_test::THRESHOLD_1E_6);
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, ComputePointCloudNearestNeighborDistance)
{
    vector<double> ref =
    {
        115.403443,  127.737235,  113.386920,  160.257386,  134.367386,
         84.927090,  104.713960,  125.367587,  131.299365,  174.718976,
        135.903713,  119.976930,   23.200313,   71.130812,  122.388130,
        149.534713,  206.804657,  167.712979,  139.532917,  130.417954,
        183.393615,  177.513260,  145.151024,  125.612685,   29.865777,
        104.860721,   80.940264,  216.433643,   43.315141,  115.403443,
         84.927090,  149.637522,  135.903713,  174.101748,  144.034256,
        241.072830,  112.365088,  161.880529,  116.398605,  177.686443,
         92.966820,   96.138437,   23.529412,  161.880529,   68.149597,
        148.191715,  150.610701,  235.849019,  125.367587,  182.342399,
        103.829038,  104.713960,   92.966820,   71.130812,  171.251808,
        156.960754,  285.736804,  171.251808,   68.149597,  118.493686,
        119.976930,   99.903837,  100.823217,  188.316976,  110.223289,
         91.968936,  137.030629,   91.968936,  126.588332,  127.737235,
         36.787575,  131.123557,  177.902686,   36.787575,   23.200313,
         99.903837,  134.367386,  177.686443,  154.392311,  122.388130,
        192.476725,  113.386920,  183.686877,  197.990675,  192.356839,
         83.465869,  148.191715,   96.218385,  147.254771,  114.265116,
        116.398605,   83.465869,  123.576088,  156.960754,   43.315141,
         29.865777,  110.223289,   23.529412,  162.070418,  179.623255
    };

    int size = 100;

    open3d::PointCloud pc;

    Eigen::Vector3d vmin(0.0, 0.0, 0.0);
    Eigen::Vector3d vmax(1000.0, 1000.0, 1000.0);

    pc.points_.resize(size);
    unit_test::Rand(pc.points_, vmin, vmax, 0);

    vector<double> distance = open3d::ComputePointCloudNearestNeighborDistance(pc);

    EXPECT_EQ(ref.size(), distance.size());
    for (size_t i = 0; i < distance.size(); i++)
        EXPECT_NEAR(ref[i], distance[i], unit_test::THRESHOLD_1E_6);
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
TEST(PointCloud, CreatePointCloudFromDepthImage)
{
    vector<Eigen::Vector3d> ref =
    {
        {  -15.709662,  -11.776101,   25.813999 },
        {  -31.647980,  -23.798088,   52.167000 },
        {   -7.881257,   -5.945074,   13.032000 },
        {  -30.145872,  -22.811805,   50.005001 },
        {  -21.734044,  -16.498585,   36.166000 },
        {  -25.000724,  -18.662512,   41.081001 },
        {  -20.246287,  -15.160878,   33.373001 },
        {  -36.219190,  -27.207171,   59.889999 },
        {  -28.185984,  -21.239675,   46.754002 },
        {  -23.713580,  -17.926114,   39.459999 },
        {   -9.505886,   -7.066190,   15.620000 },
        {  -31.858493,  -23.756333,   52.514000 },
        {  -15.815128,  -11.830214,   26.150999 },
        {   -4.186843,   -3.141786,    6.945000 },
        {   -8.614051,   -6.484428,   14.334000 },
        {  -33.263298,  -24.622128,   54.658001 },
        {  -11.742641,   -8.719418,   19.356001 },
        {  -20.688904,  -15.410790,   34.209999 },
        {  -38.349551,  -28.656141,   63.612999 },
        {  -30.197857,  -22.636429,   50.250000 },
        {  -30.617229,  -22.567629,   50.310001 },
        {  -35.316494,  -26.113137,   58.214001 },
        {  -13.822439,  -10.252549,   22.856001 },
        {  -36.237141,  -26.963181,   60.109001 },
        {  -37.240419,  -27.797524,   61.969002 }
    };

    open3d::Image image;

    // test image dimensions
    const int local_width = 5;
    const int local_height = 5;
    const int local_num_of_channels = 1;
    const int local_bytes_per_channel = 2;

    image.PrepareImage(local_width,
                       local_height,
                       local_num_of_channels,
                       local_bytes_per_channel);

    unit_test::Rand(image.data_, 0, 255, 0);

    open3d::PinholeCameraIntrinsic intrinsic =
        open3d::PinholeCameraIntrinsic(
            open3d::PinholeCameraIntrinsicParameters::PrimeSenseDefault);

    auto output_pc = open3d::CreatePointCloudFromDepthImage(image, intrinsic);

    EXPECT_EQ(ref.size(), output_pc->points_.size());
    for (size_t i = 0; i < output_pc->points_.size(); i++)
        unit_test::ExpectEQ(ref[i], output_pc->points_[i]);
}

// ----------------------------------------------------------------------------
// Test CreatePointCloudFromRGBDImage for the following configurations:
// index | color_num_of_channels | color_bytes_per_channel
//     1 |          3            |            1
//     0 |          1            |            4
// ----------------------------------------------------------------------------
void TEST_CreatePointCloudFromRGBDImage(
    const int& color_num_of_channels,
    const int& color_bytes_per_channel,
    const vector<Eigen::Vector3d>& ref_points,
    const vector<Eigen::Vector3d>& ref_colors)
{
    open3d::Image image;
    open3d::Image color;

    const int size = 5;

    // test image dimensions
    const int image_width = size;
    const int image_height = size;
    const int image_num_of_channels = 1;
    const int image_bytes_per_channel = 1;

    const int color_width = size;
    const int color_height = size;

    image.PrepareImage(image_width,
                       image_height,
                       image_num_of_channels,
                       image_bytes_per_channel);

    color.PrepareImage(color_width,
                       color_height,
                       color_num_of_channels,
                       color_bytes_per_channel);

    unit_test::Rand(image.data_, 100, 150, 0);
    unit_test::Rand(color.data_, 130, 200, 0);

    auto depth = open3d::ConvertDepthToFloatImage(image);

    open3d::RGBDImage rgbd_image(color, *depth);

    open3d::PinholeCameraIntrinsic intrinsic =
        open3d::PinholeCameraIntrinsic(
            open3d::PinholeCameraIntrinsicParameters::PrimeSenseDefault);

    auto output_pc = open3d::CreatePointCloudFromRGBDImage(rgbd_image, intrinsic);

    EXPECT_EQ(ref_points.size(), output_pc->points_.size());
    EXPECT_EQ(ref_colors.size(), output_pc->colors_.size());
    for (size_t i = 0; i < output_pc->points_.size(); i++)
    {
        unit_test::ExpectEQ(ref_points[i], output_pc->points_[i]);
        unit_test::ExpectEQ(ref_colors[i], output_pc->colors_[i]);
    }
}

// ----------------------------------------------------------------------------
// Test CreatePointCloudFromRGBDImage for the following configuration:
// color_num_of_channels = 3
// color_bytes_per_channel = 1
// ----------------------------------------------------------------------------
TEST(PointCloud, CreatePointCloudFromRGBDImage_3_1)
{
    vector<Eigen::Vector3d> ref_points =
    {
        {   -0.000337,   -0.000252,    0.000553 },
        {   -0.000283,   -0.000213,    0.000467 },
        {   -0.000330,   -0.000249,    0.000545 },
        {   -0.000329,   -0.000249,    0.000545 },
        {   -0.000342,   -0.000259,    0.000569 },
        {   -0.000260,   -0.000194,    0.000427 },
        {   -0.000276,   -0.000207,    0.000455 },
        {   -0.000327,   -0.000246,    0.000541 },
        {   -0.000267,   -0.000201,    0.000443 },
        {   -0.000299,   -0.000226,    0.000498 },
        {   -0.000294,   -0.000218,    0.000482 },
        {   -0.000312,   -0.000232,    0.000514 },
        {   -0.000280,   -0.000209,    0.000463 },
        {   -0.000296,   -0.000222,    0.000490 },
        {   -0.000346,   -0.000261,    0.000576 },
        {   -0.000346,   -0.000256,    0.000569 },
        {   -0.000312,   -0.000231,    0.000514 },
        {   -0.000320,   -0.000238,    0.000529 },
        {   -0.000253,   -0.000189,    0.000420 },
        {   -0.000306,   -0.000230,    0.000510 },
        {   -0.000239,   -0.000176,    0.000392 },
        {   -0.000264,   -0.000195,    0.000435 },
        {   -0.000251,   -0.000186,    0.000416 },
        {   -0.000331,   -0.000246,    0.000549 },
        {   -0.000252,   -0.000188,    0.000420 }
    };

    vector<Eigen::Vector3d> ref_colors =
    {
        {    0.737255,    0.615686,    0.721569 },
        {    0.725490,    0.756863,    0.560784 },
        {    0.600000,    0.717647,    0.584314 },
        {    0.658824,    0.639216,    0.678431 },
        {    0.607843,    0.647059,    0.768627 },
        {    0.756863,    0.682353,    0.701961 },
        {    0.545098,    0.674510,    0.513725 },
        {    0.572549,    0.545098,    0.729412 },
        {    0.549020,    0.619608,    0.545098 },
        {    0.537255,    0.780392,    0.568627 },
        {    0.647059,    0.737255,    0.674510 },
        {    0.588235,    0.682353,    0.650980 },
        {    0.643137,    0.776471,    0.588235 },
        {    0.717647,    0.650980,    0.717647 },
        {    0.619608,    0.752941,    0.584314 },
        {    0.603922,    0.729412,    0.760784 },
        {    0.525490,    0.768627,    0.650980 },
        {    0.529412,    0.560784,    0.690196 },
        {    0.752941,    0.603922,    0.525490 },
        {    0.513725,    0.631373,    0.525490 },
        {    0.572549,    0.772549,    0.756863 },
        {    0.741176,    0.580392,    0.654902 },
        {    0.611765,    0.713725,    0.647059 },
        {    0.690196,    0.654902,    0.517647 },
        {    0.627451,    0.764706,    0.764706 }
    };

    const int color_num_of_channels = 3;
    const int color_bytes_per_channel = 1;

    TEST_CreatePointCloudFromRGBDImage(
        color_num_of_channels,
        color_bytes_per_channel,
        ref_points,
        ref_colors);
}

// ----------------------------------------------------------------------------
// Test CreatePointCloudFromRGBDImage for the following configuration:
// color_num_of_channels = 1
// color_bytes_per_channel = 4
// ----------------------------------------------------------------------------
TEST(PointCloud, CreatePointCloudFromRGBDImage_1_4)
{
    vector<Eigen::Vector3d> ref_points =
    {
        {   -0.000337,   -0.000252,    0.000553 },
        {   -0.000283,   -0.000213,    0.000467 },
        {   -0.000330,   -0.000249,    0.000545 },
        {   -0.000329,   -0.000249,    0.000545 },
        {   -0.000342,   -0.000259,    0.000569 },
        {   -0.000260,   -0.000194,    0.000427 },
        {   -0.000276,   -0.000207,    0.000455 },
        {   -0.000327,   -0.000246,    0.000541 },
        {   -0.000267,   -0.000201,    0.000443 },
        {   -0.000299,   -0.000226,    0.000498 },
        {   -0.000294,   -0.000218,    0.000482 },
        {   -0.000312,   -0.000232,    0.000514 },
        {   -0.000280,   -0.000209,    0.000463 },
        {   -0.000296,   -0.000222,    0.000490 },
        {   -0.000346,   -0.000261,    0.000576 },
        {   -0.000346,   -0.000256,    0.000569 },
        {   -0.000312,   -0.000231,    0.000514 },
        {   -0.000320,   -0.000238,    0.000529 },
        {   -0.000253,   -0.000189,    0.000420 },
        {   -0.000306,   -0.000230,    0.000510 },
        {   -0.000239,   -0.000176,    0.000392 },
        {   -0.000264,   -0.000195,    0.000435 },
        {   -0.000251,   -0.000186,    0.000416 },
        {   -0.000331,   -0.000246,    0.000549 },
        {   -0.000252,   -0.000188,    0.000420 }
    };

    vector<Eigen::Vector3d> ref_colors =
    {
        {   -0.000352,   -0.000352,   -0.000352 },
        {   -0.000018,   -0.000018,   -0.000018 },
        {   -0.000000,   -0.000000,   -0.000000 },
        {  -24.580862,  -24.580862,  -24.580862 },
        {   -0.000000,   -0.000000,   -0.000000 },
        {   -0.001065,   -0.001065,   -0.001065 },
        {   -0.000000,   -0.000000,   -0.000000 },
        {   -0.020211,   -0.020211,   -0.020211 },
        {   -0.000000,   -0.000000,   -0.000000 },
        {   -0.000018,   -0.000018,   -0.000018 },
        {   -4.959918,   -4.959918,   -4.959918 },
        {  -93.301918,  -93.301918,  -93.301918 },
        {   -0.000000,   -0.000000,   -0.000000 },
        {   -0.000000,   -0.000000,   -0.000000 },
        {   -0.000000,   -0.000000,   -0.000000 },
        {   -0.094615,   -0.094615,   -0.094615 },
        {   -0.000005,   -0.000005,   -0.000005 },
        {   -0.000000,   -0.000000,   -0.000000 },
        {   -0.000000,   -0.000000,   -0.000000 },
        {   -0.000000,   -0.000000,   -0.000000 },
        {   -1.254324,   -1.254324,   -1.254324 },
        {   -4.550016,   -4.550016,   -4.550016 },
        {   -0.000000,   -0.000000,   -0.000000 },
        {  -80.370476,  -80.370476,  -80.370476 },
        {  -22.216120,  -22.216120,  -22.216120 }
    };

    const int color_num_of_channels = 1;
    const int color_bytes_per_channel = 4;

    TEST_CreatePointCloudFromRGBDImage(
        color_num_of_channels,
        color_bytes_per_channel,
        ref_points,
        ref_colors);
}
