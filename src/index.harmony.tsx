/*
 * Copyright (c) 2024 Huawei Device Co., Ltd. All rights reserved
 * Use of this source code is governed by a MIT license that can be
 * found in the LICENSE file.
 */

import React from 'react';
import { IRNHoleView, RNHoleView } from 'react-native-hole-view';

export const RNHoleViewHarmony: React.FC<IRNHoleView> = (props) => <RNHoleView pointerEvents='none' {...props}></RNHoleView>;
