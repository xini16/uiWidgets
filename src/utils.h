#pragma once
inline int getIndex(int rawIndex) {
  return rawIndex / 2;
  /*返回文件夹内插入点的index。因为插入点和资源都是qtreewidgetitem
  所以/2就是资源的实际index*/
}