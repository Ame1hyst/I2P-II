        int **arr = create_walkway(n, len);
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < len[i]; ++j)
            {
                scanf("%d", arr[i] + j);
            }