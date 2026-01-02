import re

def remove_duplicate_copyright_comments(file_path):
    """
    删除重复的版权声明注释
    策略：找到所有版权声明，只保留第一个，删除后续所有
    """
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()

    # 匹配C风格多行注释（/* ... */）
    comment_pattern = re.compile(r'/\*.*?\*/', re.DOTALL)
    all_comments = comment_pattern.findall(content)
    
    # 找出所有版权声明注释（基于关键特征）
    copyright_comments = []
    for comment in all_comments:
        # 放宽检测条件，只要是版权声明就捕获
        if 'Copyright' in comment and ('Permission' in comment or 'THE SOFTWARE IS PROVIDED' in comment):
            copyright_comments.append(comment)
    
    if len(copyright_comments) <= 1:
        print(f"No duplicate copyright comments found in {file_path}")
        return
    
    print(f"Found {len(copyright_comments)} copyright comments in {file_path}")
    
    # 策略：只保留第一个版权声明，删除后续所有
    # 这样可以确保无论年份、文件名如何变化，都只保留第一个
    comments_to_remove = copyright_comments[1:]  # 第一个保留，其他都删除
    
    new_content = content
    for comment in comments_to_remove:
        # 精确替换：使用正则表达式确保只替换一次
        escaped_comment = re.escape(comment)
        new_content = re.sub(escaped_comment, '', new_content, count=1)
    
    # 清理格式：删除多余的空行
    # 删除连续3个以上的空行
    new_content = re.sub(r'\n\s*\n\s*\n+', '\n\n', new_content)
    # 删除开头的空行
    new_content = new_content.lstrip('\n')
    
    with open(file_path, 'w', encoding='utf-8') as f:
        f.write(new_content)
    
    print(f"Removed {len(comments_to_remove)} duplicate copyright comments from {file_path}")