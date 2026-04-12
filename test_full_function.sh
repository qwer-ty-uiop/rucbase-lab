#!/bin/bash

# RucBase 完整功能测试脚本

echo "=========================================="
echo "  RucBase 完整功能测试"
echo "=========================================="

PROJECT_DIR="/mnt/e/Code/Project/rucbase-lab"
BUILD_DIR="$PROJECT_DIR/build"
CLIENT_DIR="$PROJECT_DIR/rucbase_client/build"
TEST_DB="test_functionality"

# 清理旧的测试数据
echo ""
echo ">>> 清理旧测试数据..."
rm -rf "$BUILD_DIR/$TEST_DB"

# 启动服务器（后台）
echo ""
echo ">>> 启动数据库服务器..."
cd "$BUILD_DIR"
./bin/rmdb "$TEST_DB" &
SERVER_PID=$!
echo "Server PID: $SERVER_PID"

# 等待服务器启动
sleep 3

# 检查服务器是否运行
if kill -0 $SERVER_PID 2>/dev/null; then
    echo "[OK] Server is running"
else
    echo "[ERROR] Server failed to start"
    exit 1
fi

# 执行SQL测试
echo ""
echo ">>> 执行SQL功能测试..."
echo ""

cd "$CLIENT_DIR"

./rmdb_client << 'SQLEOF'
show tables;
create table student (id int, name char(20), age int, score float);
show tables;
desc student;
insert into student values (1, 'Alice', 20, 95.5);
insert into student values (2, 'Bob', 21, 88.0);
insert into student values (3, 'Charlie', 22, 92.3);
insert into student values (4, 'David', 23, 76.8);
insert into student values (5, 'Eve', 20, 89.2);
select * from student;
select id, name from student where age > 20;
create index idx_student_id (id);
show index student;
select * from student where id = 3;
update student set score = 96.5 where name = 'Alice';
select * from student where name = 'Alice';
delete from student where id = 5;
select count(*) from student;
begin;
insert into student values (6, 'Frank', 24, 85.0);
commit;
select * from student;
exit;
SQLEOF

CLIENT_EXIT_CODE=$?
echo ""
echo "Client exit code: $CLIENT_EXIT_CODE"

# 停止服务器
echo ""
echo ">>> 停止服务器..."
kill $SERVER_PID 2>/dev/null || true
wait $SERVER_PID 2>/dev/null || true
echo "[OK] Server stopped"

echo ""
echo "=========================================="
echo "  测试完成"
echo "=========================================="
