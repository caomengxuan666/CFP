# 使用rust官方镜像作为构建环境
FROM rust:1.70 as builder

WORKDIR /app

# 复制Cargo文件
COPY Cargo.toml Cargo.lock ./

# 创建一个虚拟的src目录以使cargo命令正常工作
RUN mkdir src && echo "fn main() { println!(\"Dummy build\"); }" > src/main.rs

# 下载依赖
RUN cargo build --release
RUN rm src/main.rs

# 复制源代码
COPY src ./src

# 编译发布版本
RUN cargo build --release

# 使用debian slim作为运行环境
FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# 从构建器复制可执行文件
COPY --from=builder /app/target/release/crash_handler_server .

# 创建必要的目录
RUN mkdir -p data storage/minidumps storage/pdbs config logs

# 暴露端口
EXPOSE 3000

# 启动命令
CMD ["./crash_handler_server"]