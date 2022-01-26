from nis import match
from pathlib import Path
from PIL import Image
import numpy as np
from joblib import Parallel, delayed


import colmapeasy

def main(flags):
    assert flags.image_path.exists()

    images = [
        Image.open(path).convert('L')
        for path in flags.image_path.glob('*.jpg')
    ]
    
    result = Parallel(n_jobs=1, prefer="threads", require='sharedmem')(
        delayed(colmapeasy.extract_features)(
            image,
            random_seed='2022',
            SiftExtraction__num_threads='12',
            SiftExtraction__use_gpu='0',
        )
        for image in images
    )

    keypoints, descriptors = zip(*result)
    
    print(len(keypoints), len(descriptors))

    result2 = colmapeasy.match_exhaustive(
        descriptors,
        random_seed='2022',
        SiftMatching__use_gpu='0',
    )

    print('Result2 ', len(result2))
    matches_map = result2

    result3 = colmapeasy.import_matches(
        keypoints,
        matches_map,
    )

    tw_geometries = result3
    print(result3)

    # result4 = colmapeasy.map_incremental(
    #     tw_geometries
    # )

    # print(result4)


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('--image_path', type=Path)
    parser.set_defaults(image_path=Path('test_data/Keeb/images'))
    args = parser.parse_args()

    main(args)