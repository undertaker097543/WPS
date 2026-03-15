#include <QImage>
#include <QColor>
#include <QVector>
#include <QPoint>
#include <QQueue>
#include <iostream>
#include <cmath>
#include <map>

// 区域数据结构
struct RegionInfo {
    int id;
    QColor color;
    int area; // 像素数量
};

// 计算颜色距离
double colorDistance(const QColor& c1, const QColor& c2) {
    return std::sqrt(std::pow(c1.red() - c2.red(), 2) +
                     std::pow(c1.green() - c2.green(), 2) +
                     std::pow(c1.blue() - c2.blue(), 2));
}

void processImage(const QString& path) {
    QImage img(path);
    if (img.isNull()) {
        std::cout << "Failed to load image!" << std::endl;
        return;
    }

    int width = img.width();
    int height = img.height();
    // 访问标记位，优化性能
    QVector<bool> visited(width * height, false);
    QVector<RegionInfo> regions;
    
    const double THRESHOLD = 30.0; // 颜色接近阈值
    int regionCounter = 0;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = y * width + x;
            if (visited[idx]) continue;

            // 发现新区域，开始 BFS
            regionCounter++;
            QColor baseColor = img.pixelColor(x, y);
            int currentArea = 0;
            
            QQueue<QPoint> queue;
            queue.enqueue(QPoint(x, y));
            visited[idx] = true;

            while (!queue.isEmpty()) {
                QPoint curr = queue.dequeue();
                currentArea++;

                // 检查上下左右 4 个邻域（也可扩展为 8 邻域）
                static const int dx[] = {0, 0, 1, -1};
                static const int dy[] = {1, -1, 0, 0};

                for (int i = 0; i < 4; ++i) {
                    int nx = curr.x() + dx[i];
                    int ny = curr.y() + dy[i];
                    int nIdx = ny * width + nx;

                    if (nx >= 0 && nx < width && ny >= 0 && ny < height && !visited[nIdx]) {
                        if (colorDistance(baseColor, img.pixelColor(nx, ny)) < THRESHOLD) {
                            visited[nIdx] = true;
                            queue.enqueue(QPoint(nx, ny));
                        }
                    }
                }
            }
            regions.append({regionCounter, baseColor, currentArea});
        }
    }

    // 输出结果
    std::cout << "Total Regions Found: " << regions.size() << std::endl;
    for (const auto& r : regions) {
        printf("Region #%d | Color: (R:%d, G:%d, B:%d) | Area: %d pixels\n", 
               r.id, r.color.red(), r.color.green(), r.color.blue(), r.area);
    }
}

int main(int argc, char *argv[]) {
    // 简单的控制台 Qt 环境
    QCoreApplication a(argc, argv);
    processImage("pic.jpg"); // 替换为你的图片路径
    return 0;
}