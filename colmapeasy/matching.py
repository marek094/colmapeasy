from gettext import npgettext
from colmapeasy_impl import match

from joblib import Parallel, delayed
import numpy as np


def match_exhaustive(descriptors_vec, block_size=50, **kwargs):
    num = len(descriptors_vec)
    num_blocks = np.ceil(num / block_size).astype(np.int)

    def block_image_pairs(range1, range2):
        image_pairs = []
        for idx1 in range(*range1):
            for idx2 in range(*range2):
                block_id1 = idx1 % block_size
                block_id2 = idx2 % block_size
                cond1 = (idx1 > idx2) and (block_id1 <= block_id2)
                cond2 = (idx1 < idx2) and (block_id1 < block_id2)
                if cond1 or cond2:
                    image_pairs.append((idx1, idx2))
        return image_pairs

    def match_pairs(image_pairs):
        result = Parallel(n_jobs=-1, prefer="threads", require='sharedmem')(
            delayed(match)(
                descriptors_vec[idx1],
                descriptors_vec[idx2],
                **kwargs)
            for idx1, idx2 in image_pairs
        )
        return dict(zip(image_pairs, result))

    matches = {}
    for start_idx1 in range(0, num, block_size):
        end_idx1 = min(num, start_idx1 + block_size)
        for start_idx2 in range(0, num, block_size):
            end_idx2 = min(num, start_idx2 + block_size)

            print(
                "Matching block",
                f"[{start_idx1//block_size+1}/{num_blocks},",
                f"{start_idx2//block_size+1}/{num_blocks}]",
                flush=True
            )

            pairs = block_image_pairs(
                range1=(start_idx1, end_idx1), 
                range2=(start_idx2, end_idx2)
            )
            matches.update(match_pairs(pairs))

    return matches