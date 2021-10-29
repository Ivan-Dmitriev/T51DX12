if (Id == IDC_MY3D)
      {
        BITMAP bitmap;
        HDC hMemDC;
        HGDIOBJ oldBitmap;

        hMemDC = CreateCompatibleDC(DrawItem->hDC);
        oldBitmap = SelectObject(hMemDC, hImAND);

        GetObject(hImAND, sizeof(bitmap), &bitmap);
        BitBlt(DrawItem->hDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hMemDC, 0, 0, SRCAND);

        oldBitmap = SelectObject(hMemDC, hImXOR);

        GetObject(hImXOR, sizeof(bitmap), &bitmap);
        BitBlt(DrawItem->hDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hMemDC, 0, 0, SRCINVERT);
      }