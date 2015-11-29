/**
 *	�ļ���core.h
 *	�㷨������
 *
 *	Made by Luo Mingyuan
 *	Email: lmy0217@126.com
 *	���˼���¦����ʦ
 *	2015-11-28
 */


#pragma once
#include <math.h>
#include <queue>


/*
���ÿ������Χ�������
������
	mapInfo			�������־����
	row				��������
	col				��������
�޷���ֵ
*/
void getMineCount(unsigned short int *&mapInfo, int row, int col)
{
	unsigned short int *colHisto = new unsigned short int[col];
	unsigned short int coreHisto;
	int pno = 0, padd = 0, psub = 0;

	for (int j = 0; j < col; j++)
		colHisto[j] = 0;

	for (int i = 0; i < row; i++)
	{
		while (padd < (i + 2) * col)
		{
			if (i < row - 1)
				colHisto[padd % col] += ((mapInfo[padd] & 0xF000) == MF_MINE) ? 1 : 0;
			if (i > 1 && (mapInfo[psub++] & 0xF000) == MF_MINE)
				colHisto[padd % col]--;
			padd++;
		}
		coreHisto = 0;
		for (int j = 0; j < col; j++)
		{
			while (pno < j + 2 + i * col)
			{
				if (j < col - 1)
					coreHisto += colHisto[pno % col];
				if (j > 1)
					coreHisto -= colHisto[(pno - 3) % col];
				pno++;
			}
			if ((mapInfo[pno - 2] & 0xF000) != MF_MINE)
				mapInfo[pno - 2] = (mapInfo[pno - 2] & 0x00FF) + coreHisto * 0x1000;
		}
		pno--;
	}
}

/*
�������, ��ȥ��ʼ�㵽�Ŀ鼰��Χ�Ŀ�
������
	mapInfo			�������־����
	seed			�������
	firstLButtonNo	��ʼ�㵽�Ŀ����
	row				��������
	col				��������
	count			��������
�޷���ֵ
*/
void initMineCount(unsigned short int *&mapInfo, unsigned int seed, int firstLButtonNo, int row, int col, int count)
{
	int brickNumber = row * col;
	int n = 0, r;

	srand(seed);//�����������
	//�ų��㵽�Ŀ鼰����Χ�������
	while (n < count)
	{
		r = rand() % brickNumber;
		if ((mapInfo[r] & 0xF000) != MF_MINE
			&& r != firstLButtonNo
			&& r != firstLButtonNo - col
			&& r != firstLButtonNo - col + 1
			&& r != firstLButtonNo + 1
			&& r != firstLButtonNo + col + 1
			&& r != firstLButtonNo + col
			&& r != firstLButtonNo + col - 1
			&& r != firstLButtonNo - 1
			&& r != firstLButtonNo - col - 1)
		{
			mapInfo[r] = (mapInfo[r] & 0x00FF) + MF_MINE;
			n++;
		}
	}

	getMineCount(mapInfo, row, col);//����ÿ������Χ������

}

/*
������Ϸ��������������־����
������
	mapInfo			�������־����
	saveFlags		�������Ϸ����
	brickNumber		��������
�޷���ֵ
*/
void save2map(unsigned short int *&mapInfo, TCHAR *&saveFlags, int brickNumber)
{
	for (int i = 0; i < brickNumber; i++)
		switch (TCHAR temp = saveFlags[i])
		{
		case '0':case '3':case '6':
			mapInfo[i] = (mapInfo[i] & 0xF000) + ((mapInfo[i] & 0x00FF) + (temp - '0')) % 9;
			break;
		case '9':
			mapInfo[i] = (mapInfo[i] & 0xF000) + MF_NUMBER(mapInfo[i] / 0x1000);
			break;
		}
}

/*
�������־������㱣����Ϸ����
������
	saveFlags		�������Ϸ����
	mapInfo			�������־����
	brickNumber		��������
�޷���ֵ
*/
void map2save(TCHAR *&saveFlags, unsigned short int *&mapInfo, int brickNumber)
{
	for (int i = 0; i < brickNumber; i++)
		switch (mapInfo[i] & 0x0F00)
		{
		case 0x0000:
			saveFlags[i] = '0' + ((mapInfo[i] & 0x00FF) / 3 * 3 + 6) % 9;
			break;
		case 0x0100:
			saveFlags[i] = '9';
			break;
		}
	saveFlags[brickNumber] = '\0';
}

/*
��ʼ������Ч��
������
	row				��������
	col				��������
����ֵ��
	firstPaintNo	��ʼ��������ͼ����������
*/
int* getFirstPaintNo(int row, int col)
{
	int *firstPaintNo = new int[row * col];
	int startNo[4] = { 0,
		col - 1 - col % 2,
		(row - 1 - row % 2) * col,
		(row - row % 2) * col - 1 - col % 2 };
	int max = (row >= col) ? row : col;
	int min = (row >= col) ? col : row;
	int ratio = (int)ceil((float)max / (float)min);
	int n = 0, k;

	for (int i = 0; i < 2 * max; i++)
	{
		for (int j = 0; j < 2 * min; j++)
		{
			if (i / ratio >= j)
			{
				for (k = 0; k < 4; k++)
				{
					if (2 * (i - ratio * j) <= max - 1 - ((max == col) ? (k % 2) : (k / 2))
						&& 2 * j <= min - 1 - ((max == col) ? (k / 2) : (k % 2)))
					{
						firstPaintNo[n++] = startNo[k]
							+ ((max == col) ? ((k % 2) ? -1 : 1) : ((k / 2) ? -1 : 1)) 
							* (i - ratio * j) * 2 * ((max == col) ? 1 : col)
							+ ((max == col) ? ((k / 2) ? -1 : 1) : ((k % 2) ? -1 : 1))
							* j * 2 * ((max == col) ? col : 1);
					}
				}
			}
		}
	}

	return firstPaintNo;
}


/*
˫���к���
	�ж�����
������
	nextPoint		��һ���������Ŀ�
	startPoint		��ʼ�㵽�Ŀ�
	threshold		�ж���ֵ
	row				��������
	col				��������
����ֵ��
	�Ƿ���������
*/
bool require(int nextPoint, int startPoint, float threshold, int row, int col)
{
	int pointX = nextPoint % col;
	int pointY = nextPoint / col;
	int startPointX = startPoint % col;
	int startPointY = startPoint / col;
	return (pointY - startPointY) * (pointY - startPointY)
		+ (pointX - startPointX) * (pointX - startPointX) <= threshold;//�����Ƿ�С�ڰ뾶
}

/*
˫���к���
	�жϱ߽�
������
	nextType		��һ���������Ŀ������
	point			���ڱ����Ŀ�
	row				��������
	col				��������
����ֵ��
	�Ƿ�Խ���߽�
*/
bool edge(int nextType, int point, int row, int col)
{
	bool edge[4] = { point >= col, (point % col) < (col - 1),
		point < (row - 1) * col, (point % col) > 0 };
	return (nextType % 2 == 0) ? edge[nextType / 2]
		: (edge[(nextType - 1) / 2] && edge[(nextType + 1) % 8 / 2]);
}

/*
˫���к���
	��ʼ����������
������
	startPoint		��ʼ�㵽�Ŀ�
	flagid			������α��
	fast			�����
	slow			������
	no				��������������
	row				��������
	col				��������
�޷���ֵ
*/
void dbQueueInit(int startPoint, int flagid, std::queue<int> &fast,
	std::queue<int> &slow, int *&no, int row, int col)
{
	//�㵽�Ŀ鼰����Χ8�������������
	for (int k = 0; k < 8; k++)
	{
		int dc = (k % 4 == 0) ? 0 : ((k > 4) ? -1 : 1);
		int dr = (k % 4 == 0) ? (k / 2 - 1) : ((k > 4) ? (2 - k % 4) : (k % 4 - 2));
		int nextPoint = startPoint + dc + dr * col;
		if (edge(k, startPoint, row, col))
		{
			slow.push(nextPoint);
			slow.push(k);
			no[nextPoint + row * col] = flagid;
		}
	}
}

/*
˫���к���
	����˫����
������
	point			���ڱ����Ŀ�
	type			���ڱ����Ŀ������
	startPoint		��ʼ�㵽�Ŀ�
	threshold		�ж���ֵ
	flagid			������α��
	fast			�����
	slow			������
	no				��������������
	row				��������
	col				��������
�޷���ֵ
*/
void dbQueueRun(int point, int type, int startPoint, float threshold, int flagid,
	std::queue<int> &fast, std::queue<int> &slow, int *&no, int row, int col)
{
	/*int start = 0, end = 5;
	if (point < startPoint && (startPoint - point) % col == 0)
	{
	start = 2;
	end = 3;
	}*/
	for (int k = 0; k < 5; k++)//���²������5������
	{
		int nextType = (type + 6 + k) % 8;//����, ����
		int dc = (nextType % 4 == 0) ? 0 : ((nextType > 4) ? -1 : 1);
		int dr = (nextType % 4 == 0) ? (nextType / 2 - 1) : ((nextType > 4) ? (2 - nextType % 4) : (nextType % 4 - 2));
		int nextPoint = point + dc + dr * col;//���
		if (edge(nextType, point, row, col) && no[nextPoint + row * col] == 0)
		{
			if (require(nextPoint, startPoint, threshold, row, col))//�ж�����
			{
				fast.push(nextPoint);
				fast.push(nextType);
			}
			else
			{
				slow.push(nextPoint);
				slow.push(nextType);
			}
			no[nextPoint + row * col] = flagid;
		}
	}
}

/*
˫���к���
	˫���в�α���, ʵ��ƽ��Բ����ɢ
������
	startPoint		��ʼ�㵽�Ŀ�
	endRequire		��������
	mapInfo			�������־����
	row				��������
	col				��������
����ֵ��
	no				��������������
*/
int* dbQueue(int startPoint, int endRequire, unsigned short int *&mapInfo, int row, int col)
{
	std::queue<int> fast, slow;
	float r = 0;//��ʼ�뾶0
	int point, type;
	int i = 0, flagid = 1;

	int *no = new int[2 * row * col];
	for (int k = 0; k < row * col; k++) {
		no[k] = -1;
		no[k + row * col] = 0;
	}

	dbQueueInit(startPoint, flagid, fast, slow, no, row, col);

	while (!slow.empty() || !fast.empty())
	{
		while (!fast.empty())
		{
			point = fast.front();
			fast.pop();
			type = fast.front();
			fast.pop();

			if (endRequire == 0)//��������0���忪û���׵Ŀ�
			{
				no[i++] = point;
				if ((mapInfo[point] & 0xF000) != MF_ZEROMINE)
					continue;
			}
			else if (endRequire == 1)//��������1���ױ�ը
			{
				if ((mapInfo[point] & 0xF000) == MF_MINE)
					no[i++] = point;
			}

			dbQueueRun(point, type, startPoint, r * r, flagid, fast, slow, no, row, col);
		}

		r = r + 0.618f;//�뾶����
		if (!slow.empty() && require(slow.front(), startPoint, r * r, row, col))
			flagid++;//������μ�1

		while (!slow.empty() && require(slow.front(), startPoint, r * r, row, col))
		{
			point = slow.front();
			slow.pop();
			type = slow.front();
			slow.pop();

			if (endRequire == 0)//��������0���忪û���׵Ŀ�
			{
				no[i++] = point;
				if ((mapInfo[point] & 0xF000) != MF_ZEROMINE)
					continue;
			}
			else if (endRequire == 1)//��������1���ױ�ը
			{
				if ((mapInfo[point] & 0xF000) == MF_MINE)
					no[i++] = point;
			}

			dbQueueRun(point, type, startPoint, r * r, flagid, fast, slow, no, row, col);
		}
	}

	return no;
}


/*
�㵽����Ϊ��Ŀ�, ��ó忪�������
������
	startPoint		��ʼ�㵽�Ŀ�
	mapInfo			�������־����
	row				��������
	col				��������
����ֵ��
	�忪�����������
*/
int* getZeroDevelopNo(int startPoint, unsigned short int *&mapInfo, int row, int col)
{
	return dbQueue(startPoint, 0, mapInfo, row, col);
}

/*
�㵽��, ����ױ�ը��ͼ������
������
	startPoint		��ʼ�㵽����
	mapInfo			�������־����
	bmpNumber		��ըЧ����ͼ��
	row				��������
	col				��������
	count			��������
����ֵ��
	test			�ױ�ը��ͼ����������
*/
int* getMineDevelopNo(int startPoint, unsigned short int *&mapInfo, int bmpNumber, int row, int col, int count)
{
	int *no = dbQueue(startPoint, 1, mapInfo, row, col);

	std::queue<int> run;
	int np = 0, n = 0;//np	ͬһ���������Ԫ�ظ���
	int t = 0, i = 0, f = 0;//f	�������
	int d, type;//type	��ը�ڼ�����ͼ

	int *test = new int[3 * count * bmpNumber];
	for (int k = 0; k < 3 * count * bmpNumber; k++) {
		test[k] = -1;
	}

	//��һ��Ԫ�ؽ�����
	if (i < row * col && no[i] != -1)
	{
		run.push(no[i]);
		run.push(1);
		f = no[no[i] + row * col];
		np++;//�������Ԫ�ظ�����1
		i++;
	}
	//ͬһ��������ε�Ԫ�ؽ�����
	while (i < row * col && no[i] != -1 && no[no[i] + row * col] == f)
	{
		run.push(no[i]);
		run.push(1);
		np++;//�������Ԫ�ظ�����1
		i++;
	}
	if (i < row * col && no[i] != -1)
	{
		f = no[no[i] + row * col];
	}

	while (!run.empty())
	{
		d = run.front();
		run.pop();
		type = run.front();
		run.pop();

		test[t] = d;//λ�����
		test[t + count * bmpNumber] = type;//��ը�ڼ�����ͼ
		test[t + 2 * count * bmpNumber] = run.size();//����Ԫ�ظ���, �������Ʊ�ըЧ����ʱ
		t++;

		if (type < bmpNumber)
		{
			run.push(d);
			run.push(type + 1);
		}

		if (type == 1)//��һ����ͼ
		{
			np--;//����������Ԫ�ظ�����1
			if (i < row * col && no[i] != -1 && no[no[i] + row * col] == f)//��һ��������ε�һ��Ԫ�ؽ�����
			{
				run.push(no[i]);
				run.push(1);
				n++;
				i++;
			}
			if (np == 0)//�������Ԫ�ظ���Ϊ0
			{
				while (i < row * col && no[i] != -1 && no[no[i] + row * col] == f)//��һ���������ʣ�µ�Ԫ�ؽ�����
				{
					run.push(no[i]);
					run.push(1);
					n++;
					i++;
				}
				if (i < row * col && no[i] != -1)
				{
					f = no[no[i] + row * col];
				}
				np = n;
				n = 0;
			}
		}
	}

	return test;
}


/*
˫������ʱ, �����Χ������
������
	point			˫�����µĿ�
	mapInfo			�������־����
	row				��������
	col				��������
����ֵ��
	flagNumber		��Χ������
*/
int dbButtonClickFlag(int point, unsigned short int *&mapInfo, int row, int col)
{
	int no, flagNumber = 0;
	for (int i = -1; i <= 1; i++)
	{
		if (point % col + i >= 0 && point % col + i < col)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (point / col + j >= 0 && point / col + j < row)
				{
					no = point + i + j * col;
					if (no >= 0 && no < row * col && (mapInfo[no] & 0x0F00) == 0x0000 && (mapInfo[no] & 0x00FF) / 3 == 2)
					{
						flagNumber++;
					}
				}
			}
		}
	}
	return flagNumber;
}

/*
˫������ʱ, �����Χ����
������
	point			˫�����µĿ�
	mapInfo			�������־����
	row				��������
	col				��������
����ֵ��
	����Χ����ĳ���������Ҳ��Ǻ���, �򷵻����������, ���򷵻�0
*/
int dbButtonClickMine(int point, unsigned short int *&mapInfo, int row, int col)
{
	int no;
	for (int i = -1; i <= 1; i++)
	{
		if (point % col + i >= 0 && point % col + i < col)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (point / col + j >= 0 && point / col + j < row)
				{
					no = point + i + j * col;
					if (no >= 0 && no < row * col && (mapInfo[no] & 0xF000) == MF_MINE && (mapInfo[no] & 0x00FF) / 3 != 2)
					{
						return i + j * col;
					}
				}
			}
		}
	}
	return 0;
}


/*
charת��Ϊtchar
������
	_char			��Ҫת����char����
	tchar			ת�������tchar����
*/
void CharToTchar(const char *_char, TCHAR *tchar)
{
	int iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
}
