import re

def remove_duplicate_copyright_comments(file_path):
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()

    # 匹配 /* ... */ 注释块
    comment_pattern = re.compile(r'/\*.*?\*/', re.DOTALL)
    comments = comment_pattern.findall(content)

    seen = set()
    new_content = content

    for comment in comments:
        # 提取版权主体部分（忽略尾部元信息）
        main_part_match = re.search(
            r'(Copyright © \d{4}.*?THE SOFTWARE IS PROVIDED “AS IS”[^\n]*)',
            comment,
            re.DOTALL | re.IGNORECASE
        )
        if main_part_match:
            main_part = main_part_match.group(1)
            # 去掉多余空格和换行，只保留核心内容
            normalized = re.sub(r'\s+', ' ', main_part.strip())
            if normalized in seen:
                # 删除重复的版权注释
                new_content = new_content.replace(comment, '', 1)
            else:
                seen.add(normalized)

    with open(file_path, 'w', encoding='utf-8') as f:
        f.write(new_content)

    print(f"Processed {file_path}")