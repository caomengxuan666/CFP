import json
import os
import sys
from pyecharts.charts import Pie, Bar, Tab
from pyecharts import options as opts
from pyecharts.globals import ThemeType
from jinja2 import Template

# ===================== 配置项（可自定义）=====================
# 报告输出目录（自动创建）
REPORT_DIR = "clang-tidy-report"
# 中文字体配置（pyecharts 内置）
FONT_FAMILY = "Microsoft YaHei"
# ============================================================

def create_report_dir():
    """创建报告目录（包含子目录）"""
    if not os.path.exists(REPORT_DIR):
        os.makedirs(REPORT_DIR)
    # 图表输出目录
    chart_dir = os.path.join(REPORT_DIR, "charts")
    if not os.path.exists(chart_dir):
        os.makedirs(chart_dir)
    return chart_dir

def generate_interactive_charts(data, chart_dir):
    """生成交互式图表（pyecharts）"""
    # 1. 违规规则分布饼图
    checker_data = data["warnings_by_checker"]
    if checker_data:  # 检查是否有警告数据
        top_checkers = dict(sorted(checker_data.items(), key=lambda x: x[1], reverse=True)[:10])
        # 简化规则名（只保留最后部分）
        pie_data = [(k.split('.')[-1], v) for k, v in top_checkers.items()]
    else:
        # 如果没有警告，显示一个表示"无警告"的扇形
        pie_data = [("无警告", 1)]
    
    pie = (
        Pie(init_opts=opts.InitOpts(theme=ThemeType.LIGHT, width="100%", height="600px"))
        .add(
            series_name="违规规则",
            data_pair=pie_data,
            radius=["30%", "75%"],
            center=["50%", "50%"],
            rosetype="radius"
        )
        .set_global_opts(
            title_opts=opts.TitleOpts(
                title="Clang-Tidy 违规规则分布（前10）",
                title_textstyle_opts=opts.TextStyleOpts(font_size=16, font_family=FONT_FAMILY)
            ),
            legend_opts=opts.LegendOpts(
                orient="vertical",
                pos_top="15%",
                pos_left="2%",
                textstyle_opts=opts.TextStyleOpts(font_size=12, font_family=FONT_FAMILY)
            )
        )
        .set_series_opts(
            label_opts=opts.LabelOpts(
                formatter="{b}: {c} ({d}%)",
                font_size=11,
                font_family=FONT_FAMILY
            )
        )
    )
    pie.render(os.path.join(chart_dir, "checker_dist.html"))

    # 2. 文件违规数排行柱状图
    file_warnings = {}
    for file_name, warns in data["files"].items():
        file_warnings[file_name] = len(warns)
    # 筛选有警告的文件，取前15个
    file_warnings = dict(sorted(file_warnings.items(), key=lambda x: x[1], reverse=True))
    file_warnings = {k: v for k, v in file_warnings.items() if v > 0}
    # 检查是否有文件包含警告
    if file_warnings:
        top_files = dict(list(file_warnings.items())[:15])
        bar_data = [(k, v) for k, v in top_files.items()]
    else:
        # 如果没有警告，显示一个空的图表或提示信息
        bar_data = [("无警告", 0)]
    
    bar = (
        Bar(init_opts=opts.InitOpts(theme=ThemeType.LIGHT, width="100%", height="600px"))
        .add_xaxis([x[0] for x in bar_data])
        .add_yaxis("违规数", [x[1] for x in bar_data])
        .reversal_axis()  # 横向柱状图（更适合长文件名）
        .set_global_opts(
            title_opts=opts.TitleOpts(
                title="Clang-Tidy 文件违规数排行（前15）",
                title_textstyle_opts=opts.TextStyleOpts(font_size=16, font_family=FONT_FAMILY)
            ),
            xaxis_opts=opts.AxisOpts(name="违规数", name_textstyle_opts=opts.TextStyleOpts(font_family=FONT_FAMILY)),
            yaxis_opts=opts.AxisOpts(name="文件名称", name_textstyle_opts=opts.TextStyleOpts(font_family=FONT_FAMILY)),
            datazoom_opts=[opts.DataZoomOpts(type_="slider", orient="horizontal")],
        )
        .set_series_opts(
            label_opts=opts.LabelOpts(
                position="right",
                font_size=10,
                font_family=FONT_FAMILY
            )
        )
    )
    bar.render(os.path.join(chart_dir, "file_ranking.html"))

def generate_html_report(data, chart_dir):
    """生成主 HTML 报告（带 CSS 样式）"""
    # HTML 模板（包含 CSS 样式）
    html_template = """
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Clang-Tidy 检查报告</title>
    <style>
        /* 全局样式 */
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            font-family: {{ font_family }}, Arial, sans-serif;
        }
        body {
            background-color: #f5f7fa;
            color: #333;
            line-height: 1.6;
            padding: 20px;
        }
        .container {
            max-width: 1200px;
            margin: 0 auto;
            background-color: #fff;
            border-radius: 8px;
            box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
            padding: 30px;
        }
        /* 标题样式 */
        .report-title {
            text-align: center;
            font-size: 24px;
            font-weight: 600;
            color: #2c3e50;
            margin-bottom: 30px;
            padding-bottom: 15px;
            border-bottom: 2px solid #3498db;
        }
        /* 统计面板样式 */
        .stats-panel {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
            gap: 20px;
            margin-bottom: 40px;
        }
        .stat-card {
            background-color: #f8f9fa;
            border-radius: 6px;
            padding: 20px;
            text-align: center;
            border-left: 4px solid #3498db;
            transition: transform 0.2s;
        }
        .stat-card:hover {
            transform: translateY(-5px);
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
        }
        .stat-value {
            font-size: 32px;
            font-weight: 700;
            color: #2c3e50;
            margin: 10px 0;
        }
        .stat-label {
            font-size: 14px;
            color: #7f8c8d;
        }
        /* 图表区域样式 */
        .charts-section {
            margin-bottom: 40px;
        }
        .section-title {
            font-size: 18px;
            font-weight: 600;
            color: #2c3e50;
            margin-bottom: 20px;
            padding-left: 10px;
            border-left: 3px solid #3498db;
        }
        .chart-container {
            margin-bottom: 30px;
            border-radius: 6px;
            overflow: hidden;
            box-shadow: 0 2px 8px rgba(0, 0, 0, 0.08);
        }
        /* 违规规则排行样式 */
        .checker-ranking {
            margin-bottom: 40px;
        }
        .ranking-table {
            width: 100%;
            border-collapse: collapse;
            background-color: #fff;
            border-radius: 6px;
            overflow: hidden;
            box-shadow: 0 2px 8px rgba(0, 0, 0, 0.08);
        }
        .ranking-table th {
            background-color: #3498db;
            color: #fff;
            padding: 12px 15px;
            text-align: left;
            font-weight: 600;
        }
        .ranking-table td {
            padding: 12px 15px;
            border-bottom: 1px solid #eee;
        }
        .ranking-table tr:hover {
            background-color: #f8f9fa;
        }
        .ranking-table tr:nth-child(even) {
            background-color: #fafafa;
        }
        /* 文件详情样式 */
        .file-details {
            margin-top: 40px;
        }
        .file-item {
            margin-bottom: 15px;
            padding: 15px;
            background-color: #f8f9fa;
            border-radius: 6px;
        }
        .file-name {
            font-weight: 600;
            color: #2c3e50;
            margin-bottom: 10px;
        }
        .warning-item {
            padding-left: 20px;
            margin-bottom: 8px;
            color: #e67e22;
        }
        .warning-location {
            font-size: 12px;
            color: #7f8c8d;
            margin-left: 10px;
        }
        .warning-message {
            font-size: 14px;
            margin-top: 5px;
            color: #333;
        }
    </style>
</head>
<body>
    <div class="container">
        <!-- 报告标题 -->
        <h1 class="report-title">Clang-Tidy 代码检查报告</h1>

        <!-- 统计面板 -->
        <div class="stats-panel">
            <div class="stat-card">
                <div class="stat-label">总检查文件数</div>
                <div class="stat-value">{{ total_files_checked }}</div>
            </div>
            <div class="stat-card">
                <div class="stat-label">有警告文件数</div>
                <div class="stat-value">{{ files_with_warnings }}</div>
            </div>
            <div class="stat-card">
                <div class="stat-label">总警告数</div>
                <div class="stat-value">{{ total_warnings }}</div>
            </div>
            <div class="stat-card">
                <div class="stat-label">违规规则数</div>
                <div class="stat-value">{{ checker_count }}</div>
            </div>
        </div>

        <!-- 交互式图表区域 -->
        <div class="charts-section">
            <h2 class="section-title">违规规则分布</h2>
            <div class="chart-container">
                <iframe src="charts/checker_dist.html" width="100%" height="600px" frameborder="0"></iframe>
            </div>

            <h2 class="section-title">文件违规数排行</h2>
            <div class="chart-container">
                <iframe src="charts/file_ranking.html" width="100%" height="600px" frameborder="0"></iframe>
            </div>
        </div>

        <!-- 违规规则排行 -->
        <div class="checker-ranking">
            <h2 class="section-title">违规规则排行（前10）</h2>
            <table class="ranking-table">
                <tr>
                    <th>排名</th>
                    <th>规则名称</th>
                    <th>违规次数</th>
                </tr>
                {% for idx, (checker, count) in enumerate(top_checkers.items()) %}
                <tr>
                    <td>{{ idx + 1 }}</td>
                    <td>{{ checker.split('.')[-1] }}</td>
                    <td>{{ count }}</td>
                </tr>
                {% endfor %}
            </table>
        </div>

        <!-- 文件违规详情 -->
        <div class="file-details">
            <h2 class="section-title">文件违规详情（有警告的文件）</h2>
            {% for file_name, warnings in files_with_warns.items() %}
            <div class="file-item">
                <div class="file-name">{{ file_name }} （共 {{ len(warnings) }} 个警告）</div>
                {% for warn in warnings %}
                <div class="warning-item">
                    规则: {{ warn.check_name.split('.')[-1] }}
                    <span class="warning-location">{{ warn.file }}:{{ warn.line }}:{{ warn.column }}</span>
                    <div class="warning-message">{{ warn.message }}</div>
                </div>
                {% endfor %}
            </div>
            {% endfor %}
        </div>
    </div>
</body>
</html>
    """

    # 处理数据
    total_files_checked = data["total_files_checked"]
    files_with_warnings = data["files_with_warnings"]
    total_warnings = data["total_warnings"]
    checker_count = len(data["warnings_by_checker"])
    
    # 违规规则排行（前10）
    top_checkers = dict(sorted(data["warnings_by_checker"].items(), key=lambda x: x[1], reverse=True)[:10])
    
    # 有警告的文件详情
    files_with_warns = {}
    for file_name, warnings in data["files"].items():
        if len(warnings) > 0:
            files_with_warns[file_name] = warnings

    # 渲染模板
    template = Template(html_template)
    html_content = template.render(
        font_family=FONT_FAMILY,
        total_files_checked=total_files_checked,
        files_with_warnings=files_with_warnings,
        total_warnings=total_warnings,
        checker_count=checker_count,
        top_checkers=top_checkers,
        files_with_warns=files_with_warns,
        enumerate=enumerate,
        len=len
    )

    # 保存 HTML 文件
    report_path = os.path.join(REPORT_DIR, "clang_tidy_report.html")
    with open(report_path, 'w', encoding='utf-8') as f:
        f.write(html_content)
    
    print(f"✅ HTML 报告已生成：{os.path.abspath(report_path)}")

def main(json_file):
    """主函数：读取 JSON，生成图表和 HTML 报告"""
    # 1. 读取 JSON 数据
    if not os.path.exists(json_file):
        print(f"❌ 错误：JSON 文件 {json_file} 不存在！")
        sys.exit(1)
    
    with open(json_file, 'r', encoding='utf-8') as f:
        try:
            data = json.load(f)
        except json.JSONDecodeError:
            print(f"❌ 错误：JSON 文件 {json_file} 格式无效！")
            sys.exit(1)

    # 2. 创建报告目录
    chart_dir = create_report_dir()

    # 3. 生成交互式图表
    print("🔧 正在生成交互式图表...")
    generate_interactive_charts(data, chart_dir)

    # 4. 生成 HTML 报告
    print("🔧 正在生成 HTML 报告...")
    generate_html_report(data, chart_dir)

    # 5. 打印统计信息
    print("\n===== Clang-Tidy 检查统计 =====")
    print(f"总检查文件数：{data['total_files_checked']}")
    print(f"有警告的文件数：{data['files_with_warnings']}")
    print(f"总警告数：{data['total_warnings']}")
    print("\n===== 违规最多的规则 =====")
    for checker, count in sorted(data["warnings_by_checker"].items(), key=lambda x: x[1], reverse=True)[:5]:
        print(f"{checker}: {count} 次")
    
    print(f"\n🎉 报告生成完成！所有文件已保存到：{os.path.abspath(REPORT_DIR)}")
    print(f"📄 主报告文件：{os.path.abspath(os.path.join(REPORT_DIR, 'clang_tidy_report.html'))}")

if __name__ == '__main__':
    # 从命令行获取 JSON 文件路径，默认使用 clang_tidy_results.json
    if len(sys.argv) > 1:
        json_file = sys.argv[1]
    else:
        json_file = 'clang_tidy_results.json'
    
    # 检查 JSON 文件是否在根目录，若不在则提示
    if not os.path.exists(json_file):
        # 尝试从报告目录查找（可选）
        alt_json = os.path.join(REPORT_DIR, json_file)
        if os.path.exists(alt_json):
            json_file = alt_json
        else:
            print(f"❌ 错误：未找到 JSON 文件 {json_file}")
            sys.exit(1)
    
    main(json_file)