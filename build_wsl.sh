#!/bin/bash

# RucBase WSL 编译脚本
# 用于在WSL 2 Ubuntu环境中编译RucBase项目

set -e  # 遇到错误立即退出

echo "=========================================="
echo "  RucBase WSL 编译环境配置脚本"
echo "=========================================="

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

print_status() {
    echo -e "${GREEN}[✓]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[!]${NC} $1"
}

print_error() {
    echo -e "${RED}[✗]${NC} $1"
}

# 步骤 1: 检查并安装编译依赖
echo ""
echo ">>> 步骤 1/5: 检查编译依赖..."
echo ""

if ! command -v gcc &> /dev/null; then
    print_warning "正在安装编译依赖..."
    sudo apt-get update
    sudo apt-get install -y \
        build-essential \
        cmake \
        g++ \
        flex \
        bison \
        libreadline-dev \
        git \
        make
    print_status "依赖安装完成"
else
    print_status "GCC已安装: $(gcc --version | head -n1)"
fi

# 检查CMake版本
if ! command -v cmake &> /dev/null; then
    print_warning "正在安装CMake..."
    sudo apt-get install -y cmake
fi

CMAKE_VERSION=$(cmake --version | head -n1 | awk '{print $3}')
print_status "CMake版本: $CMAKE_VERSION"

# 检查GCC版本
GCC_VERSION=$(gcc --version | head -n1)
print_status "$GCC_VERSION"

# 步骤 2: 进入项目目录（Windows路径转换）
echo ""
echo ">>> 步骤 2/5: 定位项目目录..."
echo ""

# Windows的E盘在WSL中挂载为/mnt/e
PROJECT_DIR="/mnt/e/Code/Project/rucbase-lab"

if [ ! -d "$PROJECT_DIR" ]; then
    print_error "项目目录不存在: $PROJECT_DIR"
    print_error "请确认Windows路径: E:\\Code\\Project\\rucbase-lab"
    exit 1
fi

cd "$PROJECT_DIR"
print_status "进入目录: $PROJECT_DIR"

# 步骤 3: 创建构建目录
echo ""
echo ">>> 步骤 3/5: 创建构建目录..."
echo ""

BUILD_DIR="build"

if [ -d "$BUILD_DIR" ]; then
    print_warning "构建目录已存在，清理旧文件..."
    rm -rf "$BUILD_DIR"/*
else
    mkdir -p "$BUILD_DIR"
fi

print_status "构建目录: $BUILD_DIR"

# 步骤 4: CMake配置
echo ""
echo ">>> 步骤 4/5: CMake配置..."
echo ""

cd "$BUILD_DIR"

print_status "运行CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Debug

if [ $? -ne 0 ]; then
    print_error "CMake配置失败！"
    exit 1
fi

print_status "CMake配置成功"

# 步骤 5: 编译
echo ""
echo ">>> 步骤 5/5: 编译项目..."
echo ""

CORE_COUNT=$(nproc)
print_status "使用 $CORE_COUNT 个CPU核心进行编译..."

make -j$CORE_COUNT

if [ $? -ne 0 ]; then
    print_error "编译失败！"
    print_error "请检查上面的错误信息"
    exit 1
fi

echo ""
echo "=========================================="
echo -e "${GREEN}  🎉 编译成功！${NC}"
echo "=========================================="
echo ""
echo "可执行文件位置:"
echo "  - $PROJECT_DIR/build/bin/rucbase (主程序)"
echo "  - $PROJECT_DIR/build/bin/* (测试程序)"
echo ""
echo "运行方式:"
echo "  cd $PROJECT_DIR/build && ./bin/rmdb <db_name>"
echo ""
echo "或者直接运行测试:"
echo "  cd $PROJECT_DIR/build && ctest --output-on-failure"
echo ""
